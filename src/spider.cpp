/*
 * Spider.cpp
 *
 *  Created on: 2017年4月5日
 *      Author: caojian02
 */

#include "spider.h"
#include <curl/curl.h>
#include "utils/xpath.h"
#include "utils/uriparser.h"
#include "utils/utfconv.h"
#include "db-client.h"

Spider::Spider(ISpiderObserve* observer)
 : observer_(observer) {
}

Spider::~Spider() {
}

void Spider::SetOption(const sp_opt& option) {
	option_ = option;
}

const sp_opt& Spider::GetOption() {
	return option_;
}

void Spider::DownloadProcess() {
	CURL *curl;
	CURLcode res;

	printf("begin download, url = %s\n", option_.url.c_str());
	/* init curl */
	curl = curl_easy_init();
	PANIC(curl);

	if (curl) {
		if (!option_.user_agent.empty()) {
			curl_easy_setopt(curl, CURLOPT_USERAGENT, option_.user_agent.c_str());
		}
		if (!option_.proxy.empty()) {
			curl_easy_setopt(curl, CURLOPT_PROXY, option_.proxy.c_str());
		}
		if (option_.timeout != 0) {
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, option_.timeout);
		}
		if (!option_.cookie.empty()) {
			curl_easy_setopt(curl, CURLOPT_COOKIE, option_.cookie.c_str());
		}
		/*support redirection*/
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		curl_easy_setopt(curl, CURLOPT_URL, option_.url.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Spider::OnResponse);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* start the curl */
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}
}

void Spider::HtmlProcess() {
	if (!response_.empty()) {

		char* data = (char*) response_.c_str();

		char *get[option_.urls_num];
		// parse next urls
		int size = xpath(data, option_.reg_url.c_str(), get, option_.urls_num);

		for (int j = 0; j < size; j++) {
			printf("url=%s\n", get[j]);
		}

		// Add extracted urls to uri front queue
		if (size > 0) {
			// Join relative urls to the base, otherwise next requests are invalid.
			// 	If not joined, it contains only paths like : /torvalds/linux
			// Basestring to check againt relative links such as /torvalds/linux
			const char* base = option_.base_url.c_str();
			cspider_joinall(base, get, size);
			if (observer_) {
				observer_->OnParseUrls(this, get, size);
			}
			// URLS memory free
			xpath_free_str(get, size);
		}

		// parse contents
		if (size == 0) {
			// parse title
			char* title[1];
			int sz_title = xpath(data, option_.reg_title.c_str(), title, 1);

			char* desc[1];
			int sz_desc = xpath(data, option_.reg_desc.c_str(), desc, 1);

			char* content[100];
			int sz_content = xpath(data, option_.reg_content.c_str(), content, 100);

			article_t article;
			// lock write db

			article.article_type = option_.article_type;
			article.origin_url = option_.url;
			if (sz_title > 0) {
				article.title = title[0];
				xpath_free_str(title, sz_title);
			}
			if (sz_desc > 0) {
				article.desc = desc[0];
				xpath_free_str(desc, sz_desc);
			}
			if (sz_content > 0) {
				string combine;
				for (int i = 0; i < sz_content; i++) {
					if (strlen(content[i]) > 0) {
						combine.append(content[i]);
						combine.append("\n");
					}
				}
				printf("title=%s, desc=%s, size=%d, content=%s\n", title[0], desc[0], sz_content, combine.c_str());
				article.content = combine;
				xpath_free_str(content, sz_content);
			}
			DBMgr()->SaveArticle(article);
		}
	}
}

size_t Spider::OnResponse(void* data, size_t size, size_t nmemb, void* user_data) {
	Spider *spider = (Spider*) user_data;
	/* get the total size*/
	size_t length = size * nmemb;

	if (data == NULL)
		return (size_t) -1;
	/* save data in page */
	return spider->SetData(data, length);
}

int Spider::SetData(void* data, size_t length) {
	if (data != NULL) {
		response_.append((char*) data, length);
	}
	return length;
}

