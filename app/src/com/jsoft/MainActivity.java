package com.jsoft.xmedia;


import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;


public class MainActivity extends Activity {

	final private String TAG = "meida_suface";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		SurfaceView surfaceview = (SurfaceView) findViewById(R.id.surfaceView);
		SurfaceHolder holder = surfaceview.getHolder();
		holder.addCallback(new Callback(){

			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				Log.d(TAG,"surfaceCreated");
				_nativeSetVideoSurface(holder.getSurface());
			}

			@Override
			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
			}});
			_native_start_xbmc();
	}
	

	private native boolean _nativeSetVideoSurface(Surface surface);
	private native void _native_start_xbmc();
	static {
        System.loadLibrary("xbmc"); //load libxbmc.so
    }
}