#pragma execution_character_set("utf-8")

#include "Multi_Thread.h"
#include "mmsystem.h"
int i;

static void mainThread(void *param)
{
	Multi_Thread *multi = (Multi_Thread *)param;
	multi->run();
}

int Multi_Thread::next(int index)
{
	return (index + 1) % RING_BUFF_SIZE;
}

void Multi_Thread::init_queue()
{
	front = rear = 0;
}

void Multi_Thread::enqueue(double x, double y)
{
	if (next(rear) == front) {}

	if (rear == 0) {
		if (queue_time[RING_BUFF_SIZE - 1] != y) {
			queue_position[rear] = x;
			queue_time[rear] = y;
			rear = next(rear);
		}
	}
	else if (queue_time[rear - 1] != y) {
		queue_position[rear] = x;
		queue_time[rear] = y;
		rear = next(rear);
	}
	i = 1; /*enqueue ���Œ�ł����Ăяo���ꂽ*/
}

double Multi_Thread::dequeue(int i)
{
	double x;

	x = queue_position[i];

	return x;
}

int Multi_Thread::empty()
{
	return front == rear;
}

void Multi_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/*create Multi thread*/
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);
	yubi_flag = 0;
	state_flag = 0;

}

void Multi_Thread::set_flag() {
	yubi_flag = 1;
}

void Multi_Thread::set_flag2() {
	state_flag = 1;
}

void Multi_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;
	vad_julius_result = NULL;
}

Multi_Thread::Multi_Thread()
{
	initialize();
	init_queue();
	i = 0;
}

void Multi_Thread::vad_julius_event(MMDAgent *mmdagent, const char *str) {
	 vad_julius_result = MMDAgent_strdup(mmdagent->julius_result);
	 //mmdagent->sendMessage("vad�F������", "%s", mmdagent->julius_result);
	 //mmdagent->sendMessage("vad�F������", "%s", vad_julius_result);
}

void Multi_Thread::music_event() {
	PlaySound("piri.wav", NULL, SND_ASYNC);
}

void Multi_Thread::run()
{
	int tamesi = 0;          /*7��24���p�V�i���I�Ŏ����ō�������̂ł���̂ŁA�܂���蒼��*/
	char *moji;
	int sijigo = 0;         /*sijigo = 1 �u�����vsijigo = 2 �u�����v*/
	int dousa = 0;          /*dousa = 1 �u�Ȃ���vdousa = 2 �u�Ƃ܂�v*/
	int kakunin = 0;        /*�w���ꂠ��w�����Ȃ��̏ꍇ 0  �w���ꂠ��w��������̏ꍇ 1*/
	int state = 0;          /*��ԑJ�ڂ̏��*/
	int nod1 = 0;
	int nod2 = 0;
	int swing1 = 0;
	int swing2 = 0;
	int current_attention_point = 0; /*eye_gaze�p*/
	int pre_attention_point = 0;     /*eye_gaze�p*/
	int eye_state = 0;
	FILETIME start, end;             /*eye_gaze�p*/
	double eye_start, eye_end, now_time; /*eye_gaze�p*/

	MMDAgent *mmdagent;
	mmdagent = m_mmdagent;

	while (1) {
		/*�����̃v���O����*/

		current_attention_point = mmdagent->eye_gaze_point;


		//--------------------�����E�l�ɂ���đΘb��ς���Ƃ��� ���݂͓K���ɍ���Ă���------------------
		/*if (tamesi == 0 && mmdagent->face_pos_x != -999) {
			if (mmdagent->face_pos_x != 0) {
				mmdagent->sendMessage("detect", "%s", "person");
				tamesi = 999;
			}
		}*/
		//--------------------------------------------------------------------------

		//------------���Ȃ����E��U��------------------------------------
		if (mmdagent->eye_gaze_nod == 0) {
			nod1 = 1;
			nod2 = 0;
		}
		else if (mmdagent->eye_gaze_nod == 1) {
			nod2 = 1;
		}
		if (nod1 == 1 && nod2 == 1) {
			mmdagent->sendMessage("nod", "%s", "nodnod");
			nod1 = 0;
			nod2 = 0;
		}

		if (mmdagent->eye_gaze_swing == 0) {
			swing1 = 1;
			swing2 = 0;
		}
		else if (mmdagent->eye_gaze_swing == 1) {
			swing2 = 1;
		}

		if (swing1 == 1 && swing2 == 1) {
			mmdagent->sendMessage("swing", "%s", "kubihuri");
			swing1 = 0;
			swing2 = 0;
		}
        //---------------------------------------------------------------------------

		//---------------------������������1�b�Ԍ����Ƃ�--------------------------------------
		if (eye_state == 0) {
			if (current_attention_point == 5 && pre_attention_point != 5) {
				GetSystemTimeAsFileTime(&start);
				eye_start = (start.dwHighDateTime / 10000000.0) * 0x10000 * 0x10000 + start.dwLowDateTime / 10000000.0;
				//mmdagent->sendMessage("eye_start", "%f", eye_start);
				eye_state = 1;
			}
		}

		if (eye_state == 1) {
			if (current_attention_point == 5) {
				GetSystemTimeAsFileTime(&end);
				now_time = (end.dwHighDateTime / 10000000.0) * 0x10000 * 0x10000 + end.dwLowDateTime / 10000000.0;
				eye_end = now_time - eye_start;
				//mmdagent->sendMessage("now_time", "%f", now_time);
				//mmdagent->sendMessage("eye_end", "%f", eye_end);

				if (eye_end >= 1) {
					mmdagent->sendMessage("sisen", "%s", "left"); //����Ō��G�[�W�F���g�����F�Ɍ���
					//mmdagent->sendMessage("eye_end", "%f", eye_end);
					eye_start = 0;
					eye_end = 0;
					now_time = 0;
					eye_state = 0;
				}
				else {
					//mmdagent->sendMessage("pre_attention_point", "%d", pre_attention_point);
				}
			}
			else {
				eye_state = 0;
				//mmdagent->sendMessage("current_attention_point", "%d", current_attention_point);
				//mmdagent->sendMessage("pre_attention_point", "%d", pre_attention_point);
			}
		}
		pre_attention_point = current_attention_point;


		/*---------------------------------------------------------------------------------------*/

		if (state == 0) {
			state_flag = 0;

			if (mmdagent->julius_suuji == 1) {
				state = 1;
				mmdagent->sendMessage("state=", "1");
				//state_flag = 0;
				mmdagent->julius_suuji = 0;
			}
			/*else if (mmdagent->leap_jes == 1) {
				mmdagent->sendMessage("LEAP_EVENT", "%s", "keytap");
				mmdagent->leap_jes = 0;
			}*/
		}

		else if (state == 1) {
			if (mmdagent->kinect_flag == 1) {
				enqueue(mmdagent->kinect_hand_directionX, mmdagent->kinect_nowtime);
			}
			//�����F���J�n�������F�����ʂ��Ȃ������ꍇ
			if (mmdagent->julius_suuji == 2) {
				state = 0;
				mmdagent->sendMessage("state=", "0");
				mmdagent->julius_suuji = 0;
			}


			/*Julius �F�����ʂŎw���ꂪ�o�Ă�������*/
			if (vad_julius_result != NULL) {
				mmdagent->sendMessage("multimodal_part", "%s", vad_julius_result);
				mmdagent->sendMessage("julius_result", "%s", mmdagent->julius_result);

				//---------------�a�؏�񂪑����Ă����Ƃ��̊m�F�i�����Ŏ����j---------------------------
				//if ((moji = strstr(vad_julius_result, "�a��")) != NULL) {
				//mmdagent->sendMessage("����", "%s", "�a��");
				//}
				//-----------------------------------------------------------------------------


				if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "there")) != NULL) {
					mmdagent->sendMessage("Directive", "That");
					sijigo = 1;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "here")) != NULL) {
					mmdagent->sendMessage("Directive", "This");
					sijigo = 2;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "that")) != NULL) {
					mmdagent->sendMessage("Directive", "This");
					sijigo = 3;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "it")) != NULL) {
					mmdagent->sendMessage("Directive", "This");
					sijigo = 4;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "that")) != NULL) {
					mmdagent->sendMessage("Directive", "This");
					sijigo = 5;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "that")) != NULL) {
					mmdagent->sendMessage("Directive", "This");
					sijigo = 6;
					state = 2;
					mmdagent->sendMessage("state=", "2");
					mmdagent->julius_suuji = 0;
				}
				else {
					mmdagent->sendMessage("�w����Ȃ�", "%s", vad_julius_result);

					state = 0;
					mmdagent->sendMessage("state=", "0");
					mmdagent->julius_suuji = 0;
				}

				if ((moji = strstr(vad_julius_result, "�Ȃ�")) != NULL || (moji = strstr(vad_julius_result, "turn")) != NULL) {
					mmdagent->sendMessage("movement directive", "turn");
					mmdagent->sendMessage("flag", "%d", yubi_flag);
					dousa = 1;
				}
				/*else if ((moji = strstr(vad_julius_result, "�~�߂�")) != NULL) {
					mmdagent->sendMessage("movement directive", "stop");
					dousa = 2;
				}*/
				else if ((moji = strstr(vad_julius_result, "�����")) != NULL || (moji = strstr(vad_julius_result, "��낤")) != NULL || (moji = strstr(vad_julius_result, "��蓹")) != NULL || (moji = strstr(vad_julius_result, "��肽��")) != NULL) {
					dousa = 3;
				}
				else if ((moji = strstr(vad_julius_result, "�s����")) != NULL || (moji = strstr(vad_julius_result, "�s����")) != NULL || (moji = strstr(vad_julius_result, "�s������")) != NULL || (moji = strstr(vad_julius_result, "go")) != NULL) {
					dousa = 3;
				}
				else if ((moji = strstr(vad_julius_result, "�Ƃ߂�")) != NULL || (moji = strstr(vad_julius_result, "�Ƃ߂�")) != NULL || (moji = strstr(vad_julius_result, "�Ƃ܂���")) != NULL || (moji = strstr(vad_julius_result, "stop")) != NULL) {
					dousa = 3;
				}
				else if ((moji = strstr(vad_julius_result, "��")) != NULL || (moji = strstr(vad_julius_result, "what")) != NULL) {
					dousa = 4;
				}
				else {
					dousa = 0;
				}

				if (dousa == 0 || sijigo == 0) {
					sijigo = 0;
					dousa = 0;
					state = 0;
					mmdagent->sendMessage("state=", "0");
					mmdagent->julius_suuji = 0;
				}
				vad_julius_result = NULL;
				//mmdagent->julius_result = NULL;
			}
		}

		else if (state == 2) {
			/*�u�����v�Ɓu�Ȃ���v�̎�*/
			if (sijigo == 1 && dousa == 1) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i]  && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if(dequeue(i) < 0){
							mmdagent->sendMessage("leap+julius", "%s", "That=Lawson,Turn");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if(dequeue(i) >= 0){
							mmdagent->sendMessage("leap+julius", "%s", "That=GasStation,Turn");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "That=NotApplicable");
							dousa = 1;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Turn");
					dousa = 1;
				}
				kakunin = 0;
				sijigo = 0;
			}

			/*�u�����v�Ɓu�Ƃ߂�v�̏ꍇ*/
			/*if (sijigo == 1 && dousa == 2) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Stop");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Stop");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplicable");
							dousa = 2;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Stop");
					dousa = 2;
				}
				kakunin = 0;
				sijigo = 0;
			}*/

			/*�u�����v�Ɓu�Ȃ���v�̏ꍇ*/
			if (sijigo == 2 && dousa == 1) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Turn");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Turn");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplicable");
							dousa = 1;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Turn");
					dousa = 1;
				}
				kakunin = 0;
				sijigo = 0;
			}

			if (dousa == 3) {
				if (sijigo = 1 || sijigo == 2 || sijigo == 3 || sijigo == 4 || sijigo == 5 || sijigo == 6) {
					mmdagent->sendMessage("����", "%s", "�����");
					mmdagent->building_info = 0;
					sijigo = 0;
					dousa = 0;
					state = 0;
				}
			}

			if (dousa == 4) {
				if (sijigo = 1 || sijigo == 2 ||sijigo == 3 || sijigo == 4 || sijigo == 5 || sijigo == 6) {
					mmdagent->sendMessage("����", "%s", "��");
					mmdagent->building_info = 1;
					sijigo = 0;
					dousa = 0;
					state = 0;
				}
			}

			/*if (sijigo == 2 && dousa == 2) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Stop");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Stop");
							state = 0;
							dousa = 0;
							mmdagent->sendMessage("state=", "0");
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplilcable");
							dousa = 2;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Stop");
					dousa = 2;
				}
				kakunin = 0;
				sijigo = 0;
			}*/
			if (yubi_flag == 1) {
				state = 3;
				yubi_flag = 0;
				mmdagent->sendMessage("state=", "3");
				mmdagent->julius_suuji = 0;
			}
		}

		else if (state == 3) {

			if (state_flag == 1) {
				state = 0;
				dousa = 0;
				mmdagent->sendMessage("state=", "0");
				state_flag = 0;
			}

			if (mmdagent->julius_suuji == 1) {
				state = 4;
				mmdagent->sendMessage("state=", "4");
				mmdagent->julius_suuji = 0;
			}

			if (mmdagent->kinect_flag == 1) {
				//enqueue(mmdagent->leap_xzahyou, mmdagent->leap_nowtime);
				if (dousa == 1) {
					//if (mmdagent->leap_xzahyou > -0.65 && mmdagent->leap_xzahyou < -0.3) {
					if (mmdagent->kinect_hand_directionX < 0) {
						mmdagent->sendMessage("LEAP_EVENT", "%s", "Lawson");
						state = 0;
						mmdagent->sendMessage("state=", "0");
						dousa = 0;
						mmdagent->julius_suuji = 0;
					}
					//else if (mmdagent->leap_xzahyou > 0.1 && mmdagent->leap_xzahyou < 0.5) {
					else if (mmdagent->kinect_hand_directionX >= 0) {
						mmdagent->sendMessage("LEAP_EVENT", "%s", "GasStation");
						state = 0;
						mmdagent->sendMessage("state=", "0");
						dousa = 0;
						mmdagent->julius_suuji = 0;
					}
					else {
						mmdagent->sendMessage("LEAP_EVENT", "%s", "NotApplicable");
						state = 99;
						mmdagent->julius_suuji = 0;
					}
				}

				/*if (dousa == 2) {
					//if (mmdagent->leap_xzahyou > -0.65 && mmdagent->leap_xzahyou < -0.3) {
					if (mmdagent->kinect_hand_directionX < 0) {
						mmdagent->sendMessage("LEAP_EVENT2", "%s", "Lawson");
						state = 0;
						mmdagent->sendMessage("state=", "0");
						dousa = 0;
						mmdagent->julius_suuji = 0;
					}
					//else if (mmdagent->leap_xzahyou > 0.1 && mmdagent->leap_xzahyou < 0.5) {
					else if (mmdagent->kinect_hand_directionX >= 0) {
						mmdagent->sendMessage("LEAP_EVENT2", "%s", "GasStation");
						state = 0;
						mmdagent->sendMessage("state=", "0");
						dousa = 0;
						mmdagent->julius_suuji = 0;
					}
					else {
						mmdagent->sendMessage("LEAP_EVENT2", "%s", "NotApplicable");
						state = 99;
						mmdagent->julius_suuji = 0;
					}
				}*/
			}
		}

		else if (state == 99) {
			if (yubi_flag == 1) {
				state = 3;
				mmdagent->sendMessage("state=", "3");
				mmdagent->julius_suuji = 0;
				yubi_flag = 0;
			}
		}

		else if (state == 4) {
			if (mmdagent->kinect_flag == 1) {
				enqueue(mmdagent->kinect_hand_directionX, mmdagent->kinect_nowtime);
			}

			if (mmdagent->julius_suuji == 2) {
				state = 3;
				mmdagent->sendMessage("state=", "3");
				mmdagent->julius_suuji = 0;
			}
			/*Julius �F�����ʂŎw���ꂪ�o�Ă�������*/
			if (vad_julius_result != NULL) {
				mmdagent->sendMessage("multimodal_part", "%s", vad_julius_result);
				mmdagent->sendMessage("julius_result", "%s", mmdagent->julius_result);
				if (dousa == 1) {
					if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "there")) != NULL) {
						mmdagent->sendMessage("Directive", "That");
						sijigo = 1;
						state = 5;
						mmdagent->sendMessage("state=", "5");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "here")) != NULL) {
						mmdagent->sendMessage("Directive", "This");
						sijigo = 2;
						state = 5;
						mmdagent->sendMessage("state=", "5");
						mmdagent->julius_suuji = 0;
					}
					/*else if ((moji = strstr(vad_julius_result, "�t�@�~���[�}�[�g")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�2", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "�K�\�����X�^���h")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�2", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}*/
					else if ((moji = strstr(vad_julius_result, "��")) != NULL || (moji = strstr(vad_julius_result, "left")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�2", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "�E")) != NULL || (moji = strstr(vad_julius_result, "right")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�2", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}
					else {
						state = 3;
						mmdagent->sendMessage("state=", "3");
					}
				}

				if (dousa == 2) {
					if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "there")) != NULL) {
						mmdagent->sendMessage("Directive", "That");
						sijigo = 1;
						state = 5;
						mmdagent->sendMessage("state=", "5");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "����")) != NULL || (moji = strstr(vad_julius_result, "here")) != NULL) {
						mmdagent->sendMessage("Directive", "This");
						sijigo = 2;
						state = 5;
						mmdagent->sendMessage("state=", "5");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "�t�@�~���[�}�[�g")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�3", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}
					else if ((moji = strstr(vad_julius_result, "�K�\�����X�^���h")) != NULL) {
						mmdagent->sendMessage("�w����Ȃ�3", "%s", vad_julius_result);
						state = 0;
						dousa = 0;
						mmdagent->sendMessage("state=", "0");
						mmdagent->julius_suuji = 0;
					}
					else {
						state = 3;
						mmdagent->sendMessage("state=", "3");
					}
				}
				//mmdagent->julius_result = NULL;
				vad_julius_result = NULL;
			}
		}

		else if (state == 5) {
			/*�u�����v�Ɓu�Ȃ���v�̎�*/
			if (sijigo == 1 && dousa == 1) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "That=Lawson,Turn");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "That=GasStation,Turn");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "That=NotApplicable");
							dousa = 1;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Turn");
					dousa = 1;

				}
				kakunin = 0;
				sijigo = 0;
			}

			/*�u�����v�Ɓu�Ƃ߂�v�̏ꍇ*/
			if (sijigo == 1 && dousa == 2) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Stop");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Stop");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplicable");
							dousa = 2;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Stop");
					dousa = 2;
				}
				kakunin = 0;
				sijigo = 0;
			}

			/*�u�����v�Ɓu�Ȃ���v�̏ꍇ*/
			if (sijigo == 2 && dousa == 1) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Turn");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Turn");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplicable");
							dousa = 1;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Turn");
					dousa = 1;
				}
				kakunin = 0;
				sijigo = 0;
			}

			if (sijigo == 2 && dousa == 2) {
				mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
				mmdagent->sendMessage("julius time", "%.10f", mmdagent->julius_nowtime);

				for (i = 0; i < RING_BUFF_SIZE; i++) {
					if (mmdagent->julius_nowtime - 1.0 <= queue_time[i] && mmdagent->julius_nowtime + 1.0 > queue_time[i]) {
						kakunin = 1;
						//if (dequeue(i) > -0.65 && dequeue(i) < -0.3) {
						if (dequeue(i) < 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=Lawson,Stop");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						//else if (dequeue(i) > 0.1 && dequeue(i) < 0.5) {
						else if (dequeue(i) >= 0) {
							mmdagent->sendMessage("leap+julius", "%s", "This=GasStation,Stop");
							state = 0;
							mmdagent->sendMessage("state=", "0");
							dousa = 0;
							mmdagent->julius_suuji = 0;
						}
						else {
							mmdagent->sendMessage("leap+julius", "%s", "This=NotApplilcable");
							dousa = 2;
						}
					}
				}
				if (kakunin == 0) {
					mmdagent->sendMessage("leap+julius", "%s", "NotPoint,Stop");
					dousa = 2;
				}
				kakunin = 0;
				sijigo = 0;
			}

			if (yubi_flag == 1) {
				state = 3;
				mmdagent->sendMessage("state=", "3");
				mmdagent->julius_suuji = 0;
				yubi_flag = 0;
			}
		}
	}
	if (state_flag == 1) {
		state_flag = 0;
	}
}