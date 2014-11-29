package com.ugame.help;

import java.io.IOException;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.os.Build;
import android.os.Environment;
import android.util.Log;

import com.ugame.main.Uactivity;

public class Helper {
	private static Canvas mCanvas = null;
	private static Paint mPaint = null;
	
	public static void init(Activity act) {
		mCanvas = new Canvas();
		mPaint = new Paint();
	}
    public static Bitmap getAssetBitmap(String filename) {
    	Bitmap bmp = null;
    	try {
    		long st = System.currentTimeMillis();
			bmp = BitmapFactory.decodeStream(Uactivity.getActivity().getAssets().open(filename));
			Log.e(null, "time   load bitmap " + (System.currentTimeMillis() - st));
		} catch (IOException e) {
			e.printStackTrace();
		}
    	return bmp;
    }
    
    public static Bitmap convertToBMP(byte[] data) {
    	Bitmap bmp = null;
    	bmp = BitmapFactory.decodeByteArray(data, 0, data.length);
    	return bmp;
    }
    
    private final static byte TEXT_STYLE_FILL				= 0;
    private final static byte TEXT_STYLE_STROKE		= 1;
    private final static byte TEXT_STYLE_FILL_AND_STROKE		= 2;
    
	public static Bitmap textToBitmapPOT(String text, int[] bitmapSize, int fontSize, 
										int strokeWidth, int paintStyle,
										int red, int green, int blue, int alpha,
										int strokeRed, int strokeGreen, int strokeBlue, int strokeAlpha) {
		mPaint.reset();
		mPaint.setTextSize(fontSize);
		mPaint.setAntiAlias(true);
		FontMetrics fm = mPaint.getFontMetrics();
		int textHeight = (int)Math.ceil(fm.descent - fm.ascent);
		int textWidth = (int)Math.ceil(mPaint.measureText(text));
		textWidth = (0 == textWidth ? 1 : textWidth);	/* at least 1 pixel */ 
		///////////////////////////////////////////////////////////
		/* 计算图片POT尺寸 */ 
		boolean sizeToFit = false;
		int widthPOT = textWidth;
		if ((widthPOT != 1) && (widthPOT & (widthPOT - 1)) != 0) {
			int i = 1;
			while ((sizeToFit ? 2 * i : i) < widthPOT) {
				i = i << 1;
			}
			widthPOT = i;
		}
		if (widthPOT > 2048) {
			widthPOT = 2048;
		}
		int heightPOT = textHeight;
		if ((heightPOT != 1) && (heightPOT & (heightPOT - 1)) != 0) {
			int i = 1;
			while ((sizeToFit ? 2 * i : i) < heightPOT) {
				i = i << 1;
			}
			heightPOT = i;
		}
		if (heightPOT > 2048) {
			heightPOT = 2048;
		}
		////////////////////////////////////////////////////////////

		Bitmap bitmap = Bitmap.createBitmap(widthPOT, heightPOT, Bitmap.Config.ARGB_8888);
//		Bitmap bitmap = Bitmap.createBitmap(widthPOT, heightPOT, Bitmap.Config.ALPHA_8);
		bitmap.eraseColor(0);
		mCanvas.setBitmap(bitmap);
		mCanvas.save();
		int oy = (int) (textHeight - Math.ceil(fm.descent));
		
		switch (paintStyle) {
		case TEXT_STYLE_FILL:
			// 画实心字
			mPaint.setStyle(Paint.Style.FILL);
			mPaint.setColor(Color.argb(alpha, red, green, blue));
			mCanvas.drawText(text, 0, oy, mPaint);
			break;
		case TEXT_STYLE_STROKE:
			// 画空心字
			mPaint.setStyle(Paint.Style.STROKE);
			mPaint.setStrokeWidth(strokeWidth);
			mPaint.setColor(Color.argb(strokeAlpha, strokeRed, strokeGreen, strokeBlue));
			mCanvas.drawText(text, 0, oy, mPaint);
			break;
		case TEXT_STYLE_FILL_AND_STROKE:
			// 画空心字（包边）
			mPaint.setStyle(Paint.Style.STROKE);
			mPaint.setStrokeWidth(strokeWidth);
			mPaint.setColor(Color.argb(strokeAlpha, strokeRed, strokeGreen, strokeBlue));
			mCanvas.drawText(text, 0, oy, mPaint);
			mPaint.setStyle(Paint.Style.FILL);
			// 画实心字
			mPaint.setStyle(Paint.Style.FILL);
			mPaint.setColor(Color.argb(alpha, red, green, blue));
			mCanvas.drawText(text, 0, oy, mPaint);
			break;
		}
		mCanvas.restore();
		if (bitmapSize != null && bitmapSize.length >= 5) {
			bitmapSize[0] = textWidth;
			bitmapSize[1] = textHeight;
			bitmapSize[2] = widthPOT;
			bitmapSize[3] = heightPOT;
			bitmapSize[4] = (int) mPaint.measureText(text);
		}
		return bitmap;
	}
	
	private static String getAppName(Activity app) {
		String appName;
	    try {
	    	appName = (String)app.getPackageManager().getApplicationLabel(app.getPackageManager().getApplicationInfo(app.getPackageName(), 0));
	    } catch (NameNotFoundException e1) {
	        Log.e("R", "Error in getAppName()", e1);
	        appName = "n/a";
	    }
	    return appName;
	}
	
	private static long getVersion(Activity app) {
	    int version = 0;
	    try {
	        PackageInfo pInfo = app.getPackageManager().getPackageInfo(app.getPackageName(), PackageManager.GET_META_DATA);
	        version = pInfo.versionCode;
	    } catch (NameNotFoundException e1) {
	        Log.e("R", "Error in getVersion()", e1);
	    }
	    return (long)version;
	}
	
	public static void getSysInfo(String output[], long outputLong[]) {
		Activity app = Uactivity.getActivity();
		output[0] = Environment.getExternalStorageDirectory().getAbsolutePath();
		output[1] = Environment.getRootDirectory().getAbsolutePath();
		output[2] = Environment.getDataDirectory().getAbsolutePath();
		output[3] = Environment.getDownloadCacheDirectory().getAbsolutePath();
		output[4] = null; // reserved
		output[5] = app.getApplicationContext().getFilesDir().getAbsolutePath();
		output[6] = app.getApplicationContext().getPackageCodePath();
		output[7] = app.getPackageName();
		output[8] = app.getPackageResourcePath();
		output[9] = app.getApplicationInfo().backupAgentName;
		output[10] = app.getApplicationInfo().className;
		output[11] = app.getApplicationInfo().dataDir;
		output[12] = app.getApplicationInfo().nativeLibraryDir;
		output[13] = app.getApplicationInfo().processName;
		output[14] = app.getApplicationInfo().publicSourceDir;
		output[15] = app.getApplicationInfo().sourceDir;
		output[16] = app.getCallingPackage();
		output[17] = Build.BOARD;
		output[18] = Build.BOOTLOADER;
		output[19] = Build.BRAND;
		output[20] = Build.CPU_ABI;
		output[21] = Build.CPU_ABI2;
		output[22] = Build.DEVICE;
		output[23] = Build.DISPLAY;
		output[24] = Build.FINGERPRINT;
		output[25] = Build.HARDWARE;
		output[26] = Build.HOST;
		output[27] = Build.ID;
		output[28] = Build.MANUFACTURER;
		output[29] = Build.MODEL;
		output[30] = Build.PRODUCT;
		output[31] = Build.TAGS;
		output[32] = Build.TYPE;
		output[33] = Build.USER;
		output[34] = Build.VERSION.CODENAME;
		output[35] = Build.VERSION.RELEASE;
		output[36] = Build.VERSION.INCREMENTAL;
		output[37] = Build.VERSION.SDK;
		output[38] = getAppName(app);

		outputLong[0] = app.getTaskId();
		outputLong[1] = (Environment.isExternalStorageRemovable() ? 1 : 0);	// 0 = false, 1 = true
		outputLong[2] = getVersion(app);
		
		return;
	}
}
