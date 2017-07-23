/*
 * db-client.cpp
 *
 *  Created on: 2017年3月23日
 *      Author: caojian02
 */

#include "db-client.h"
#include <time.h>

DbClient::DbClient() {
	mongoc_init ();
	client_ = mongoc_client_new ("mongodb://localhost:27017/?appname=spider");
	articles_collection_ = mongoc_client_get_collection(client_, "cms", "articles");
	articles_ = new vector<article_t>;
}

DbClient::~DbClient() {
	mongoc_collection_destroy (articles_collection_);
	mongoc_client_destroy (client_);
	mongoc_cleanup ();

	if (articles_) {
		delete articles_;
		articles_ = NULL;
	}
}

DbClient* DbClient::GetInstance() {
	static DbClient instance;
	return &instance;
}

int DbClient::SaveArticle(const article_t& article) {
	bson_error_t error;
	bson_oid_t oid;
	bson_t *doc = bson_new ();

	bson_oid_init (&oid, NULL);
	BSON_APPEND_OID (doc, "_id", &oid);

	// BSON_APPEND_INT32 (doc, "article_type", article.article_type);
	if (!article.title.empty()) {
		BSON_APPEND_UTF8 (doc, "title", article.title.c_str());
	}
	if (!article.desc.empty()) {
		BSON_APPEND_UTF8 (doc, "desc", article.desc.c_str());
	}
	if (!article.content.empty()) {
		BSON_APPEND_UTF8 (doc, "content", article.content.c_str());
	}
	if (!article.author_id.empty()) {
		BSON_APPEND_UTF8 (doc, "author_id", article.author_id.c_str());
	}
	if (!article.author_name.empty()) {
		BSON_APPEND_UTF8 (doc, "author_name", article.author_name.c_str());
	}
	if (!article.origin_url.empty()) {
		BSON_APPEND_UTF8 (doc, "origin_url", article.origin_url.c_str());
	}
	if (!article.origin_time.empty()) {
		BSON_APPEND_UTF8 (doc, "origin_time", article.origin_time.c_str());
	}

	BSON_APPEND_INT32 (doc, "article_type", article.article_type);
	// default time
	bson_append_now_utc (doc, "create_at", -1);
	bson_append_now_utc (doc, "update_at", -1);

	if (!mongoc_collection_insert (articles_collection_, MONGOC_INSERT_NONE, doc, NULL, &error)) {
		fprintf (stderr, "%s\n", error.message);
	}

	bson_destroy (doc);

	return 0;
}

int DbClient::GetArticleCount(const string& origin_url) {
	bson_error_t error;

	bson_t *doc = bson_new ();
	if (!origin_url.empty()) {
		BSON_APPEND_UTF8 (doc, "origin_url", origin_url.c_str());
	}
	int64_t count = mongoc_collection_count (
			articles_collection_, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);

	if (count < 0) {
		fprintf (stderr, "%s\n", error.message);
	}
	bson_destroy (doc);

	return count;
}

void DbClient::ParseArticle(const bson_iter_t* iter, article_t* article) {
	/*uint32_t len = 0;
	const char* key = bson_iter_key (iter);
	if (strcmp(key, "title") == 0) {
		article->title = bson_iter_utf8(iter, &len);
		printf("title = %s ", article->title);
	} else if (strcmp(key, "desc") == 0) {
		article->desc = bson_iter_utf8(iter, &len);
		printf("desc = %s ", article->desc);
	} else if (strcmp(key, "content") == 0) {
		article->content = bson_iter_utf8(iter, &len);
		printf("content = %s \n", article->content);
	}*/
}

vector<article_t>* DbClient::GetArticles() {
	printf("get articles");
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;

	query = bson_new ();
	cursor = mongoc_collection_find_with_opts (articles_collection_, query, NULL, NULL);

	while (mongoc_cursor_next (cursor, &doc)) {
		// str = bson_as_json (doc, NULL);
		// printf ("%s\n", str);
		// bson_free (str);
		bson_iter_t iter;

		if (bson_iter_init (&iter, doc)) {
		  while (bson_iter_next (&iter)) {
			  article_t article;
			  ParseArticle(&iter, &article);
			  articles_->push_back(article);
		  }
	   }
	}

	bson_destroy (query);
	mongoc_cursor_destroy (cursor);

	return articles_;
}
