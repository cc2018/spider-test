/*
 * spider-mgr.h
 *
 *  Created on: 2017年3月27日
 *      Author: caojian02
 */

#ifndef SPIDER_MGR_H_
#define SPIDER_MGR_H_

#include "comdef.h"
#include "spider.h"
#include "utils/thpool.h"

#define DEFAULT_QUEUE_SIZE 512

class SpiderMgr : public ISpiderObserve {
public:
	SpiderMgr();
	virtual ~SpiderMgr();

	void Init();
	void Start();

	// interface
	virtual void OnParseUrls(Spider* from, char **urls, int size);

private:
	static void SpiderProc(void* data);

private:
	threadpool thpool_;
	vector<sp_opt> options_;
};

#endif /* SPIDER_MGR_H_ */
