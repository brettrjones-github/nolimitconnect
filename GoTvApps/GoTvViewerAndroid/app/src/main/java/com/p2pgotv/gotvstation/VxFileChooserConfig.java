package com.p2pgotv.gotvstation;

import android.os.Parcel;
import android.os.Parcelable;

public class VxFileChooserConfig implements Parcelable 
{
	public static final String TAG = VxFileChooserConfig.class.getSimpleName();

	public enum Mode 
	{
		Open,
		Save,
	}
	
	public Mode mode;
	public String pattern;
	public String title;
	public String subtitle;
	public String initialDir;
	
	@Override
	public int describeContents() 
	{
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) 
	{
		dest.writeInt(mode.ordinal());
		dest.writeString(pattern);
		dest.writeString(title);
		dest.writeString(subtitle);
		dest.writeString(initialDir);
	}
	
	public static final Parcelable.Creator<VxFileChooserConfig> CREATOR = new Parcelable.Creator<VxFileChooserConfig>() 
	{
		@Override
		public VxFileChooserConfig[] newArray(int size) 
		{
			return new VxFileChooserConfig[size];
		}
		
		@Override
		public VxFileChooserConfig createFromParcel(Parcel in) 
		{
			VxFileChooserConfig res = new VxFileChooserConfig();
			int mode_i = in.readInt(); if (mode_i >= 0 || mode_i < Mode.values().length) res.mode = Mode.values()[mode_i]; else res.mode = Mode.Open;
			res.pattern = in.readString();
			res.title = in.readString();
			res.subtitle = in.readString();
			res.initialDir = in.readString();
			return res;
		}
	};
}
