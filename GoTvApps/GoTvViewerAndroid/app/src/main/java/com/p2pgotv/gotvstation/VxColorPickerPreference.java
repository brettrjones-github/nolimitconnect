package com.p2pgotv.gotvstation;

import android.content.Context;
import android.content.res.TypedArray;
import android.os.Parcel;
import android.os.Parcelable;
import android.preference.Preference;
import android.util.AttributeSet;
import android.view.View;

import com.p2pgotv.gotvstation.VxColorPickerDialog;
import com.p2pgotv.gotvstation.R;

public class VxColorPickerPreference extends Preference 
{
	private final boolean supportsAlpha;
	int value;

	public VxColorPickerPreference(Context context, AttributeSet attrs) 
	{
		super(context, attrs);

		final TypedArray ta = context.obtainStyledAttributes(attrs, R.styleable.VxColorPickerPreference);
		supportsAlpha = ta.getBoolean(R.styleable.VxColorPickerPreference_supportsAlpha, false);

		setWidgetLayoutResource(R.layout.vx_color_picker_pref_widget);
	}

	@Override protected void onBindView(View view) {
		super.onBindView(view);

		// Set our custom views inside the layout
		final View box = view.findViewById(R.id.vx_color_picker_pref_widget_box);
		if (box != null) {
			box.setBackgroundColor(value);
		}
	}

	@Override protected void onClick() 
	{
		new VxColorPickerDialog(getContext(), value, supportsAlpha, new VxColorPickerDialog.VxOnColorPickerListener() 
		{
			@Override public void onOk(VxColorPickerDialog dialog, int color) 
			{
				if (!callChangeListener(color)) 
					return; // They don't want the value to be set
				value = color;
				persistInt(value);
				notifyChanged();
			}

			@Override public void onCancel(VxColorPickerDialog dialog) 
			{
				// nothing to do
			}
		}).show();
	}

	public void forceSetValue(int value) 
	{
		this.value = value;
		persistInt(value);
		notifyChanged();
	}

	@Override protected Object onGetDefaultValue(TypedArray a, int index) 
	{
		// This preference type's value type is Integer, so we read the default value from the attributes as an Integer.
		return a.getInteger(index, 0);
	}

	@Override protected void onSetInitialValue(boolean restoreValue, Object defaultValue) 
	{
		if (restoreValue) 
		{ // Restore state
			value = getPersistedInt(value);
		} 
		else 
		{ // Set state
			int value = (Integer) defaultValue;
			this.value = value;
			persistInt(value);
		}
	}

	/*
	 * Suppose a client uses this preference type without persisting. We
	 * must save the instance state so it is able to, for example, survive
	 * orientation changes.
	 */
	@Override protected Parcelable onSaveInstanceState() 
	{
		final Parcelable superState = super.onSaveInstanceState();
		if (isPersistent()) return superState; // No need to save instance state since it's persistent

		final SavedState myState = new SavedState(superState);
		myState.value = value;
		return myState;
	}

	@Override protected void onRestoreInstanceState(Parcelable state) 
	{
		if (!state.getClass().equals(SavedState.class)) 
		{
			// Didn't save state for us in onSaveInstanceState
			super.onRestoreInstanceState(state);
			return;
		}

		// Restore the instance state
		SavedState myState = (SavedState) state;
		super.onRestoreInstanceState(myState.getSuperState());
		this.value = myState.value;
		notifyChanged();
	}

	/**
	 * SavedState, a subclass of {@link BaseSavedState}, will store the state
	 * of MyPreference, a subclass of Preference.
	 * <p>
	 * It is important to always call through to super methods.
	 */
	private static class SavedState extends BaseSavedState 
	{
		int value;

		public SavedState(Parcel source) 
		{
			super(source);
			value = source.readInt();
		}

		@Override public void writeToParcel(Parcel dest, int flags) 
		{
			super.writeToParcel(dest, flags);
			dest.writeInt(value);
		}

		public SavedState(Parcelable superState) 
		{
			super(superState);
		}

		@SuppressWarnings("unused") public static final Parcelable.Creator<SavedState> CREATOR = new Parcelable.Creator<SavedState>() 
				{
			public SavedState createFromParcel(Parcel in) 
			{
				return new SavedState(in);
			}

			public SavedState[] newArray(int size) 
			{
				return new SavedState[size];
			}
		};
	}
}
