/*
 * Spider.h
 *
 *  Created on: 2017年4月5日
 *      Author: caojian02
 */

#ifndef SPIDER_H_
#define SPIDER_H_

#include "comdef.h"

typedef struct spider_option {
	string origin_type;
	string url;
	string start_url;
	string base_url; // auto parse
	string reg_url;
	string reg_title;
	string reg_desc;
	string reg_content;
	string collection;

	string user_agent;
	string cookie;
	string proxy;
	long   timeout;
	int    article_type;
	int    urls_num;
	int    depths;

	spider_option() {
		origin_type = "other";
		user_agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36";
		timeout = 15000;
		article_type = 0;
		urls_num = 20;
		depths = 10;
	}
} sp_opt;

class Spider;

class ISpiderObserve {
public:
	virtual ~ISpiderObserve() {
	};

	virtual void OnParseUrls(Spider* from, char **urls, int size) = 0;
};

class Spider {
public:
	Spider(ISpiderObserve* observer);
	virtual ~Spider();

	void SetOption(const sp_opt& option);
	const sp_opt& GetOption();

	void DownloadProcess();
	void HtmlProcess();

private:
	static size_t OnResponse(void *data, size_t size, size_t nmemb, void *user_data);
	int SetData(void *data, size_t length);

private:
	ISpiderObserve* observer_;
	sp_opt option_;
	string response_;
};

#endif /* SPIDER_H_ */
