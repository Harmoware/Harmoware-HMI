#include "MMDAgent.h"

#define RING_BUFF_SIZE 4000 //元は4000

typedef long ELEM;

//struct leap_data{
//ELEM queue_time[RING_BUFF_SIZE];
//ELEM queue_position[RING_BUFF_SIZE];
//};

class Multi_Thread
{
private:

	MMDAgent *m_mmdagent;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /*thread*/
	char *vad_julius_result;           /*vad_julius result*/
	int yubi_flag;
	int state_flag;

	/* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	//ELEM queue[RING_BUFF_SIZE];
	double queue_time[RING_BUFF_SIZE];
	double queue_position[RING_BUFF_SIZE];
	int front;
	int rear;

	/* Multi_Thread: thread constructor */
	Multi_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);

	void set_flag();

	void set_flag2();

	/* run: main loop */
	void run();

	void vad_julius_event(MMDAgent *mmdagent, const char *str);
	void music_event();

	//次の要素の添え字を求める.
	int next(int index);

	//キューを初期化する
	void init_queue();

	//キューにデータを入れる
	void enqueue(double x, double y);

	//キューからデータを取り出す.
	double dequeue(int i);

	//キューが空かどうかを調べる.
	int empty();
};