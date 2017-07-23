/*
 * spider-mgr.cpp
 *
 *  Created on: 2017年3月27日
 *      Author: caojian02
 */

#include "spider-mgr.h"
#include "utils/cJSON.h"
#include "db-client.h"

SpiderMgr::SpiderMgr() {
	thpool_ = NULL;
}

SpiderMgr::~SpiderMgr() {
}

void SpiderMgr::Init() {
	FILE* fp = fopen("/Users/caojian02/c/mongo-test/spider.json", "r");
	if (fp != NULL) {
		char buf[MAXLINE];
		string options;
		while (fgets(buf, MAXLINE, fp) != NULL) {
			options.append(buf);
		}
		fclose(fp);

		// parse JSON
		cJSON *root = NULL;
		do {
			root = cJSON_Parse(options.c_str());
			if (root == NULL) {
				cJSON_Delete(root);
				break;
			}

			cJSON *js_list = cJSON_GetObjectItem(root, "spiders");
			if (js_list == NULL)
				break;

			cJSON *item;
			cJSON *prop;
			sp_opt option;
			int array_size = cJSON_GetArraySize(js_list);
			for(int i = 0; i < array_size; i++) {
				item = cJSON_GetArrayItem(js_list, i);
				if (item == NULL) {
					continue;
				}
				prop = cJSON_GetObjectItem(item, "origin_type");
				if (prop) {
					option.origin_type = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "start_url");
				if (prop) {
					option.url = option.start_url = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "base_url");
				if (prop) {
					option.base_url = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "reg_url");
				if (prop) {
					option.reg_url = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "reg_title");
				if (prop) {
					option.reg_title = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "reg_desc");
				if (prop) {
					option.reg_desc = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "reg_content");
				if (prop) {
					option.reg_content = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "user_agent");
				if (prop) {
					option.user_agent = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "cookie");
				if (prop) {
					option.cookie = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "proxy");
				if (prop) {
					option.proxy = prop->valuestring;
				}
				prop = cJSON_GetObjectItem(item, "urls_num");
				if (prop) {
					option.urls_num = prop->valueint;
				}
				prop = cJSON_GetObjectItem(item, "article_type");
				if (prop) {
					option.article_type = prop->valueint;
				}
				options_.push_back(option);
			}
			cJSON_Delete(root);
		} while (false);
	}
}

void SpiderMgr::Start() {
	printf("begin call start");
	// cs_run(spider_);
	thpool_ = thpool_init(4);

	// add start work
	Spider* spider = NULL;
	vector<sp_opt>::iterator iter = options_.begin();
	while (iter != options_.end()) {
		spider = new Spider(this);
		spider->SetOption(*iter);
		thpool_add_work(thpool_, &SpiderMgr::SpiderProc, (void*) spider);
		iter++;
	}

	thpool_wait(thpool_);
	thpool_destroy(thpool_);

	printf("end call start");
}

void SpiderMgr::OnParseUrls(Spider* from, char **urls, int size) {
	// add spiders
	Spider* spider = NULL;
	const sp_opt& base = from->GetOption();
	sp_opt option = base;
	int count = 0;
	for (int i = 0; i < size; i++) {
		option.url = urls[i];
		count = DBMgr()->GetArticleCount(option.url);
		if (count <= 0) {
			// not exist, spider it
			spider = new Spider(this);
			spider->SetOption(option);
			thpool_add_work(thpool_, &SpiderMgr::SpiderProc, (void*) spider);
		}
	}
}

void SpiderMgr::SpiderProc(void* data) {
	Spider *spider = (Spider*) data;
	if (spider) {
		spider->DownloadProcess();
		spider->HtmlProcess();

		delete spider;
		spider = NULL;
	}
}
