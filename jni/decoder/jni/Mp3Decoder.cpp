#include "com_phuket_tour_decoder_Mp3Decoder.h"
#include "../libffmpeg_decoder/accompany_decoder_controller.h"

#define LOG_TAG "Mp3Decoder"

AccompanyDecoderController* decoderController;

JNIEXPORT jint JNICALL Java_com_phuket_tour_decoder_Mp3Decoder_init
  (JNIEnv * env, jobject obj, jstring mp3PathParam, jstring pcmPathParam) {
	const char* pcmPath = env->GetStringUTFChars(pcmPathParam, NULL);
	const char* mp3Path = env->GetStringUTFChars(mp3PathParam, NULL);
	decoderController = new AccompanyDecoderController();
	decoderController->Init(mp3Path, pcmPath);
	env->ReleaseStringUTFChars(mp3PathParam, mp3Path);
	env->ReleaseStringUTFChars(pcmPathParam, pcmPath);
}

JNIEXPORT void JNICALL Java_com_phuket_tour_decoder_Mp3Decoder_decode
  (JNIEnv * env, jobject obj) {
	LOGI("enter Java_com_phuket_tour_decoder_Mp3Decoder_decode...");
	if(decoderController) {
		decoderController->Decode();
	}
	LOGI("leave Java_com_phuket_tour_decoder_Mp3Decoder_decode...");
}

JNIEXPORT void JNICALL Java_com_phuket_tour_decoder_Mp3Decoder_destroy
  (JNIEnv * env, jobject obj) {
	if(decoderController) {
		decoderController->Destroy();
		delete decoderController;
		decoderController = NULL;
	}
}
