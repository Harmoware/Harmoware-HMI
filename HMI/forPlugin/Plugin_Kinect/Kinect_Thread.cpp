#pragma execution_character_set("utf-8")

#include "MMDAgent.h"
#include "Kinect_Thread.h"
#include "math.h"

#define PI 3.141592

FILETIME hajime2, matitime;
double kinectstart, matistart, matihikaku;
int time_flag = 0;

HRESULT hResult = S_OK;
IKinectSensor* kinect = nullptr;
IBodyFrameSource* bodySource = nullptr;
IBodyFrameReader* BodyReader = nullptr;
//ICoordinateMapper* CoordinateMapper = nullptr;
CameraSpacePoint* cameraSpacePoint;
CameraSpacePoint* Position;
IBodyFrame* bodyFrame = nullptr;
IBody* bodies[BODY_COUNT];

static void mainThread(void *param)
{
	Kinect_Thread *kinect = (Kinect_Thread *)param;
	kinect->run();
}


void Kinect_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);

	/*GetDefaultKinectSensor(&kinect);
	kinect->Open();*/

	//----kinectがつながっているかチェック------
	/*BOOLEAN isOpen = FALSE;
	kinect->get_IsOpen(&isOpen);
	if (!isOpen) {
		mmdagent->sendMessage("kinect connect error", "%s");
	}*/
	/*kinect->get_BodyFrameSource(&bodySource);
	bodySource->OpenReader(&BodyReader);*/
	//kinect->get_CoordinateMapper(&CoordinateMapper);
	/*for (auto& body : bodies) {
		body = nullptr;
	}*/
}

void Kinect_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;
}

void Kinect_Thread::clear()
{
	//CoordinateMapper->Release();
	//CoordinateMapper = nullptr;
	bodySource->Release();
	bodySource = nullptr;
	bodyFrame->Release();
	bodyFrame = nullptr;
	BodyReader->Release();
	BodyReader = nullptr;
	kinect->Close();
	kinect->Release();
}


Kinect_Thread::Kinect_Thread()
{
	initialize();
}

Kinect_Thread::~Kinect_Thread()
{
	clear();
}

void Kinect_Thread::run()
{

	MMDAgent *mmdagent;
	mmdagent = m_mmdagent;

	//double deg = 180;//270
	//double rad = deg * PI / 180.0;
	
	GetDefaultKinectSensor(&kinect);
	kinect->Open();

	kinect->get_BodyFrameSource(&bodySource);
	bodySource->OpenReader(&BodyReader);
	for (auto& body : bodies) {
		body = nullptr;
	}
	//kinect->get_CoordinateMapper(&CoordinateMapper);

	while (1) {
		//確認用Sleep:sleepを入れてからsendMessageを入れないとエラーが出て動作しない
		Sleep(2000);

		//IBodyFrame* bodyFrame = nullptr;
		hResult = BodyReader->AcquireLatestFrame(&bodyFrame);
		if (FAILED(hResult)) {
			//mmdagent->sendMessage("kinect", "%s", "失敗");
			//mmdagent->sendMessage("kinectbody", "%d", hResult);
		}
		if (SUCCEEDED(hResult)) {
			for (auto& body : bodies) {
				if (body != nullptr) {
					body->Release();
					body = nullptr;
				}
			}
			hResult = bodyFrame->GetAndRefreshBodyData(6, &bodies[0]);
		
			if(SUCCEEDED(hResult)){
				//mmdagent->sendMessage("kinectbody", "%s", "detect");
				//mmdagent->sendMessage("kinectbody", "%d", hResult);
				for (auto body : bodies) {
					if (body == nullptr) {
						continue;
					}
					BOOLEAN isTracked = false;
					body->get_IsTracked(&isTracked);
					if (!isTracked) {
						continue;
					}

					//関節の位置
					Joint joints[JointType::JointType_Count];
					body->GetJoints(JointType::JointType_Count, joints);
					//mmdagent->sendMessage("kinectbody", "%s","detect");
					for (auto joint : joints) {
						if (joint.TrackingState == TrackingState::TrackingState_Tracked) {
							//mmdagent->sendMessage("kinectbody", "%s","追跡");
							if (joint.JointType == JointType::JointType_HandTipRight) {
								if (0 < joints[JointType_HandTipRight].Position.Y && joints[JointType_HandTipRight].Position.Z < 1.3) {
									mmdagent->kinect_flag = 1; /*kinect set flag*/

									GetSystemTimeAsFileTime(&hajime2);
									kinectstart = (hajime2.dwHighDateTime / 10000000.0) * 0x10000 * 0x10000 + hajime2.dwLowDateTime / 10000000.0;
									mmdagent->kinect_nowtime = kinectstart;

									mmdagent->kinect_hand_directionX = joints[JointType_HandTipRight].Position.X;
									//-------------------------------
									//mmdagent->sendMessage("X", "%.10f", joints[JointType_HandTipRight].Position.X + 0.35);
									
									mmdagent->kinect_flag = 0;
								}
									//mmdagent->sendMessage("kinectbody", "%s", "手");
									//mmdagent->sendMessage("kinect time", "%.10f", mmdagent->kinect_nowtime);
									//mmdagent->sendMessage("X", "%.10f", (joints[JointType_HandTipRight].Position.X * cos(rad)) - (joints[JointType_HandTipRight].Position.Y * sin(rad)));
									//mmdagent->sendMessage("X", "%.10f", joints[JointType_HandTipRight].Position.X + 0.35);
									//mmdagent->sendMessage("Y", "%.10f", joints[JointType_HandTipRight].Position.Y);
									//mmdagent->sendMessage("Z", "%.10f", joints[JointType_HandTipRight].Position.Z);
							}
						}
					}
				}
			}
		}
		if (BodyReader != nullptr) {
			BodyReader->Release();
			BodyReader = nullptr;
			//kinect->get_BodyFrameSource(&bodySource);
			bodySource->OpenReader(&BodyReader);
		}
		if (bodyFrame != nullptr) {
			bodyFrame->Release();
			bodyFrame = nullptr;
		}
		

		/*IBodyFrame* bodyFrame = nullptr;
		hResult = BodyReader->AcquireLatestFrame(&bodyFrame);
		if (SUCCEEDED(hResult)) {
			IBody* pBody[BODY_COUNT] = { 0 };
			mmdagent->sendMessage("kinectbody", "%s","detect");
			hResult = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody);
			if (SUCCEEDED(hResult)) {
				//mmdagent->sendMessage("kinectbody", "%s","detect");
				for (int count = 0; count < BODY_COUNT; count++) {
					BOOLEAN bTracked = false;
					hResult = pBody[count]->get_IsTracked(&bTracked);
					if (SUCCEEDED(hResult) && bTracked) {
						//mmdagent->sendMessage("kinectbody", "%s","detect");
						Joint joint[JointType::JointType_Count];
						hResult = pBody[count]->GetJoints(JointType::JointType_Count, joint);
						if (SUCCEEDED(hResult)) {
							// Left Hand State
							HandState leftHandState = HandState::HandState_Unknown;
							hResult = pBody[count]->get_HandLeftState(&leftHandState);
							if (SUCCEEDED(hResult)) {
								//mmdagent->sendMessage("kinectbody", "%s","detect");
							}
						}
					}
				}
			}
			//mmdagent->sendMessage("kinectbody", "%s","detect");
			for (int count = 0; count < BODY_COUNT; count++) {
				//pBody[count] = nullptr;
				pBody[count]->Release();
				pBody[count] = nullptr;
			}
			//mmdagent->sendMessage("kinectbody", "%s","detect");
		}
		//bodyFrame->Release();
		bodyFrame = nullptr;
		*/

	}
}
