/*
 * Copyright (C) 2015 Emanuel Moecklin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.p2pgotv.gotvstation.rteditor.toolbar.spinner;

import android.widget.TextView;

/**
 * The spinner item for the font size.
 */
public class FontSizeSpinnerItem extends SpinnerItem {
    final private int mFontSize;
    private final boolean mIsEmpty;

    public FontSizeSpinnerItem(int size, String title, boolean isEmpty) {
        super(title);
        mFontSize = size;
        mIsEmpty = isEmpty;
    }

    public int getFontSize() {
        return mFontSize;
    }

    public boolean isEmpty() {
        return mIsEmpty;
    }

    @Override
    void formatNameView(TextView view) {
        super.formatNameView(view);
        view.setTextSize(mFontSize);
    }
}