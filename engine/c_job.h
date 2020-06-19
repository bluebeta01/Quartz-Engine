#pragma once

class Job
{
public:
	enum STATUS
	{
		IN_QUEUE, PROCESSING, DONE
	};

	STATUS status = Job::IN_QUEUE;
	virtual ~Job() {};
};