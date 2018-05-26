package com.phuket.tour.ffmpeg_decoder;

import com.phuket.tour.decoder.Mp3Decoder;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {
	static {
		System.loadLibrary("songstudio");
	}
	private static String TAG = "MainActivity";

	/** 原始的文件路径 **/
	private static String mp3FilePath = "/mnt/sdcard/131.mp3";
	/** 解码后的PCM文件路径 **/
	private static String pcmFilePath = "/mnt/sdcard/131.pcm";

	private Button mp3_encoder_btn;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mp3_encoder_btn = (Button) findViewById(R.id.mp3_encoder_btn);
		mp3_encoder_btn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				long startTimeMills = System.currentTimeMillis();
				Mp3Decoder decoder = new Mp3Decoder();
				int ret = decoder.init(mp3FilePath, pcmFilePath);
				if(ret >= 0) {
					decoder.decode();
					decoder.destroy();
				} else {
					Log.i(TAG, "Decoder Initialized Failed...");
				}
				int wasteTimeMills = (int)(System.currentTimeMillis() - startTimeMills);
				Log.i(TAG, "Decode Mp3 Waste TimeMills : " + wasteTimeMills + "ms");
			}
		});
	}

}
