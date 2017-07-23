/*
 * db-client.h
 *
 *  Created on: 2017年3月23日
 *      Author: caojian02
 */

#ifndef DB_CLIENT_H_
#define DB_CLIENT_H_
#include "comdef.h"
#include <bson.h>
#include <bcon.h>
#include <mongoc.h>

#define DBMgr() DbClient::GetInstance()

typedef struct _user {
    char* id;
    char* name;
    char* phone;
    char* image;
    char* email;
    int gender;
    int mod;
}user_t;

typedef struct _article {
	int    article_type;
	string id;
	string title;
	string desc;
	string content;
	string author_id;
	string author_name;
	string origin_url;
	string origin_time;
}article_t;

class DbClient {
public:
	virtual ~DbClient();

	static DbClient* GetInstance();
	vector<article_t>* GetArticles();

	int SaveArticle(const article_t& article);
	int GetArticleCount(const string& origin_url);

private:
	DbClient();
	void ParseArticle(const bson_iter_t* iter, article_t* article);


private:
	mongoc_client_t      *client_;
	mongoc_collection_t  *articles_collection_;
	string users_collection_;

	/* data */
	vector<article_t>*  articles_;
};

#endif /* DB_CLIENT_H_ */
