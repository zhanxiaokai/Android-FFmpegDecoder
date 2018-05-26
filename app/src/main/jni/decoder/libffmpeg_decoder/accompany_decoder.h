#ifndef ACCOMPANY_DECODER_H
#define ACCOMPANY_DECODER_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef UINT64_C
#define UINT64_C(value)__CONCAT(value,ULL)
#endif

#ifndef INT64_MIN
#define INT64_MIN  (-9223372036854775807LL - 1)
#endif

#ifndef INT64_MAX
#define INT64_MAX	9223372036854775807LL
#endif
typedef struct AudioPacket {

	static const int AUDIO_PACKET_ACTION_PLAY = 0;
	static const int AUDIO_PACKET_ACTION_PAUSE = 100;
	static const int AUDIO_PACKET_ACTION_SEEK = 101;

	short * buffer;
	int size;
	float position;
	int action;

	float extra_param1;
	float extra_param2;

	AudioPacket() {
		buffer = NULL;
		size = 0;
		position = -1;
		action = 0;
		extra_param1 = 0;
		extra_param2 = 0;
	}
	~AudioPacket() {
//		__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete AudioPacket");
		if (NULL != buffer) {
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "buffer's addr is %x", buffer);
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete buffer ...");
			delete[] buffer;
			buffer = NULL;
//			__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete buffer success");
		}
//		__android_log_print(ANDROID_LOG_ERROR, "~AudioPacket", "delete AudioPacket success");
	}
} AudioPacket;


#include "./../../common/CommonTools.h"

extern "C" {
	#include "../../3rdparty/ffmpeg/include/libavcodec/avcodec.h"
	#include "../../3rdparty/ffmpeg/include/libavformat/avformat.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/avutil.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/samplefmt.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/common.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/channel_layout.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/opt.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/imgutils.h"
	#include "../../3rdparty/ffmpeg/include/libavutil/mathematics.h"
	#include "../../3rdparty/ffmpeg/include/libswscale/swscale.h"
	#include "../../3rdparty/ffmpeg/include/libswresample/swresample.h"
};

#define OUT_PUT_CHANNELS 2

class AccompanyDecoder {
private:
	/** 如果使用了快进或者快退命令，则先设置以下参数 **/
	bool seek_req;
	bool seek_resp;
	float seek_seconds;

	float actualSeekPosition;

	AVFormatContext* avFormatContext;
	AVCodecContext * avCodecContext;
	int stream_index;
	float timeBase;
	AVFrame *pAudioFrame;
	AVPacket packet;

	char* accompanyFilePath;

	bool seek_success_read_frame_success;
	int packetBufferSize;

	/** 每次解码出来的audioBuffer以及这个audioBuffer的时间戳以及当前类对于这个audioBuffer的操作情况 **/
	short* audioBuffer;
	float position;
	int audioBufferCursor;
	int audioBufferSize;
	float duration;
	bool isNeedFirstFrameCorrectFlag;
	float firstFrameCorrectionInSecs;

	SwrContext *swrContext;
	void *swrBuffer;
	int swrBufferSize;

	int init(const char* fileString);
	int readSamples(short* samples, int size);
	int readFrame();
	bool audioCodecIsSupported();

public:
	AccompanyDecoder();
	virtual ~AccompanyDecoder();

	//获取采样率以及比特率
	virtual int getMusicMeta(const char* fileString, int * metaData);
	//初始化这个decoder，即打开指定的mp3文件
	virtual int init(const char* fileString, int packetBufferSizeParam);
	virtual AudioPacket* decodePacket();
	//销毁这个decoder
	virtual void destroy();
	void setSeekReq(bool seekReqParam){
		seek_req = seekReqParam;
		if(seek_req){
			seek_resp = false;
		}
	};
	bool hasSeekReq() {
		return seek_req;
	};
	bool hasSeekResp() {
		return seek_resp;
	};
	/** 设置到播放到什么位置，单位是秒，但是后边3位小数，其实是精确到毫秒 **/
	void setPosition(float seconds) {
		actualSeekPosition = -1;
		this->seek_seconds = seconds;
		this->seek_req = true;
		this->seek_resp = false;
	};

	float getActualSeekPosition(){
		float ret = actualSeekPosition;
		if(ret != -1){
			actualSeekPosition = -1;
		}
		return ret;
	};
	virtual void seek_frame();
};
#endif //ACCOMPANY_DECODER_H
