
package com.p2pgotv.gotvstation;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.Gravity;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * Copyright (C) 2015 Wasabeef
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
//================================================================================
// Modified for MyP2PWeb by Brett R. Jones
// brett1900@usa.com
// http://www.gotvp2p.net
//================================================================================

//================================================================================
public class VxHtmlEditorOld extends WebView
{
	public enum ETextAttrType
	{
		BOLD, ITALIC, SUBSCRIPT, SUPERSCRIPT, STRIKETHROUGH, UNDERLINE, H1, H2, H3, H4, H5, H6
	}

	public interface OnTextChangeListener
	{
		void onTextChange( String text );
	}

	public interface OnDecorationStateListener
	{
		void onStateChangeListener( String text, List<ETextAttrType> types );
	}

	public interface AfterInitialLoadListener
	{
		void onAfterInitialLoad( boolean isReady );
	}

	private static final String 		SETUP_HTML 			= "file:///android_asset/editor.html";
	private static final String 		CALLBACK_SCHEME 	= "re-callback://";
	private static final String 		STATE_SCHEME 		= "re-state://";
	
	private boolean 					m_IsReady 			= false;
	private String 						m_HtmlContents;
	private OnTextChangeListener 		m_TextChangeListener;
	private OnDecorationStateListener 	m_DecorationStateListener;
	private AfterInitialLoadListener 	m_LoadListener;

	//============================================================================
	public VxHtmlEditorOld( Context context )
	{
		this( context, null );
	}

	//============================================================================
	public VxHtmlEditorOld( Context context, AttributeSet attrs )
	{
		this( context, attrs, android.R.attr.webViewStyle );
	}

	//============================================================================
	@SuppressLint("SetJavaScriptEnabled")
	public VxHtmlEditorOld( Context context, AttributeSet attrs, int defStyleAttr )
	{
		super( context, attrs, defStyleAttr );

		setVerticalScrollBarEnabled( false );
		setHorizontalScrollBarEnabled( false );
		getSettings().setJavaScriptEnabled( true );
		setWebChromeClient( new WebChromeClient() );
		setWebViewClient( createWebviewClient() );
		loadUrl( SETUP_HTML );

		applyAttributes( context, attrs );
	}

	//============================================================================
	public void setHtml( String contents )
	{
		if( contents == null )
		{
			contents = "";
		}
		try
		{
			exec( "javascript:RE.setHtml('"
					+ URLEncoder.encode( contents, "UTF-8" ) + "');" );
		}
		catch( UnsupportedEncodingException e )
		{
			// No handling
		}

		m_HtmlContents = contents;
	}
	
	//============================================================================
	public String getHtml()
	{
		return m_HtmlContents;
	}

	//============================================================================
	protected EditorWebViewClient createWebviewClient()
	{
		return new EditorWebViewClient();
	}

	//============================================================================
	public void setOnTextChangeListener( OnTextChangeListener listener )
	{
		m_TextChangeListener = listener;
	}

	//============================================================================
	public void setOnDecorationChangeListener(
			OnDecorationStateListener listener )
	{
		m_DecorationStateListener = listener;
	}

	//============================================================================
	public void setOnInitialLoadListener( AfterInitialLoadListener listener )
	{
		m_LoadListener = listener;
	}

	//============================================================================
	private void callback( String text )
	{
		m_HtmlContents = text.replaceFirst( CALLBACK_SCHEME, "" );
		if( m_TextChangeListener != null )
		{
			m_TextChangeListener.onTextChange( m_HtmlContents );
		}
	}

	//============================================================================
	private void stateCheck( String text )
	{
		String state = text.replaceFirst( STATE_SCHEME, "" ).toUpperCase(
				Locale.ENGLISH );
		List<ETextAttrType> types = new ArrayList<ETextAttrType>();
		for( ETextAttrType type : ETextAttrType.values() )
		{
			if( TextUtils.indexOf( state, type.name() ) != -1 )
			{
				types.add( type );
			}
		}

		if( m_DecorationStateListener != null )
		{
			m_DecorationStateListener.onStateChangeListener( state, types );
		}
	}

	//============================================================================
	private void applyAttributes( Context context, AttributeSet attrs )
	{
		final int[] attrsArray = new int[]
		{ android.R.attr.gravity };
		TypedArray ta = context.obtainStyledAttributes( attrs, attrsArray );

		int gravity = ta.getInt( 0, NO_ID );
		switch( gravity )
		{
		case Gravity.LEFT:
			exec( "javascript:RE.setTextAlign(\"left\")" );
			break;
		case Gravity.RIGHT:
			exec( "javascript:RE.setTextAlign(\"right\")" );
			break;
		case Gravity.TOP:
			exec( "javascript:RE.setVerticalAlign(\"top\")" );
			break;
		case Gravity.BOTTOM:
			exec( "javascript:RE.setVerticalAlign(\"bottom\")" );
			break;
		case Gravity.CENTER_VERTICAL:
			exec( "javascript:RE.setVerticalAlign(\"middle\")" );
			break;
		case Gravity.CENTER_HORIZONTAL:
			exec( "javascript:RE.setTextAlign(\"center\")" );
			break;
		case Gravity.CENTER:
			exec( "javascript:RE.setVerticalAlign(\"middle\")" );
			exec( "javascript:RE.setTextAlign(\"center\")" );
			break;
		}

		ta.recycle();
	}

	//============================================================================
	public void setEditorFontColor( int color )
	{
		String hex = convertHexColorString( color );
		exec( "javascript:RE.setBaseTextColor('" + hex + "');" );
	}

	//============================================================================
	public void setEditorFontSize( int px )
	{
		exec( "javascript:RE.setBaseFontSize('" + px + "px');" );
	}

	//============================================================================
	@Override
	public void setPadding( int left, int top, int right, int bottom )
	{
		super.setPadding( left, top, right, bottom );
		exec( "javascript:RE.setPadding('" + left + "px', '" + top + "px', '"
				+ right + "px', '" + bottom + "px');" );
	}

	//============================================================================
	@Override
	public void setPaddingRelative( int start, int top, int end, int bottom )
	{
		// still not support RTL.
		setPadding( start, top, end, bottom );
	}

	//============================================================================
	public void setEditorBackgroundColor( int color )
	{
		setBackgroundColor( color );
	}

	//============================================================================
	@Override
	public void setBackgroundColor( int color )
	{
		super.setBackgroundColor( color );
	}

	//============================================================================
	@Override
	public void setBackgroundResource( int resid )
	{
		Bitmap bitmap = VxHtmlUtilsOld.decodeResource( getContext(), resid );
		String base64 = VxHtmlUtilsOld.toBase64( bitmap );
		bitmap.recycle();

		exec( "javascript:RE.setBackgroundImage('url(data:image/png;base64,"
				+ base64 + ")');" );
	}

	//============================================================================
	@Override
	public void setBackground( Drawable background )
	{
		Bitmap bitmap = VxHtmlUtilsOld.toBitmap( background );
		String base64 = VxHtmlUtilsOld.toBase64( bitmap );
		bitmap.recycle();

		exec( "javascript:RE.setBackgroundImage('url(data:image/png;base64,"
				+ base64 + ")');" );
	}

	//============================================================================
	public void setBackground( String url )
	{
		exec( "javascript:RE.setBackgroundImage('url(" + url + ")');" );
	}

	//============================================================================
	public void setEditorWidth( int px )
	{
		exec( "javascript:RE.setWidth('" + px + "px');" );
	}

	//============================================================================
	public void setEditorHeight( int px )
	{
		exec( "javascript:RE.setHeight('" + px + "px');" );
	}

	//============================================================================
	public void setPlaceholder( String placeholder )
	{
		exec( "javascript:RE.setPlaceholder('" + placeholder + "');" );
	}

	//============================================================================
	public void loadCSS( String cssFile )
	{
		String jsCSSImport = "(function() {"
				+ "    var head  = document.getElementsByTagName(\"head\")[0];"
				+ "    var link  = document.createElement(\"link\");"
				+ "    link.rel  = \"stylesheet\";"
				+ "    link.type = \"text/css\";" + "    link.href = \""
				+ cssFile + "\";" + "    link.media = \"all\";"
				+ "    head.appendChild(link);" + "}) ();";
		exec( "javascript:" + jsCSSImport + "" );
	}

	//============================================================================
	public void undo()
	{
		exec( "javascript:RE.undo();" );
	}

	//============================================================================
	public void redo()
	{
		exec( "javascript:RE.redo();" );
	}

	//============================================================================
	public void setBold()
	{
		exec( "javascript:RE.setBold();" );
	}

	//============================================================================
	public void setItalic()
	{
		exec( "javascript:RE.setItalic();" );
	}

	//============================================================================
	public void setSubscript()
	{
		exec( "javascript:RE.setSubscript();" );
	}

	//============================================================================
	public void setSuperscript()
	{
		exec( "javascript:RE.setSuperscript();" );
	}

	//============================================================================
	public void setStrikeThrough()
	{
		exec( "javascript:RE.setStrikeThrough();" );
	}

	//============================================================================
	public void setUnderline()
	{
		exec( "javascript:RE.setUnderline();" );
	}

	//============================================================================
	public void setTextColor( int color )
	{
		exec( "javascript:RE.prepareInsert();" );

		String hex = convertHexColorString( color );
		exec( "javascript:RE.setTextColor('" + hex + "');" );
	}

	//============================================================================
	public void setTextBackgroundColor( int color )
	{
		exec( "javascript:RE.prepareInsert();" );

		String hex = convertHexColorString( color );
		exec( "javascript:RE.setTextBackgroundColor('" + hex + "');" );
	}

	//============================================================================
	public void removeFormat()
	{
		exec( "javascript:RE.removeFormat();" );
	}

	//============================================================================
	public void setHeading( int heading )
	{
		exec( "javascript:RE.setHeading('" + heading + "');" );
	}

	//============================================================================
	public void setIndent()
	{
		exec( "javascript:RE.setIndent();" );
	}

	//============================================================================
	public void setOutdent()
	{
		exec( "javascript:RE.setOutdent();" );
	}

	//============================================================================
	public void setAlignLeft()
	{
		exec( "javascript:RE.setJustifyLeft();" );
	}

	//============================================================================
	public void setAlignCenter()
	{
		exec( "javascript:RE.setJustifyCenter();" );
	}

	//============================================================================
	public void setAlignRight()
	{
		exec( "javascript:RE.setJustifyRight();" );
	}

	//============================================================================
	public void setBlockquote()
	{
		exec( "javascript:RE.setBlockquote();" );
	}

	//============================================================================
	public void setBullets()
	{
		exec( "javascript:RE.setBullets();" );
	}

	//============================================================================
	public void setNumbers()
	{
		exec( "javascript:RE.setNumbers();" );
	}

	//============================================================================
	public void insertImage( String url, String alt )
	{
		exec( "javascript:RE.prepareInsert();" );
		exec( "javascript:RE.insertImage('" + url + "', '" + alt + "');" );
	}

	//============================================================================
	public void insertLink( String href, String title )
	{
		exec( "javascript:RE.prepareInsert();" );
		exec( "javascript:RE.insertLink('" + href + "', '" + title + "');" );
	}

	//============================================================================
	public void insertTodo()
	{
		exec( "javascript:RE.prepareInsert();" );
		exec( "javascript:RE.setTodo('" + VxHtmlUtilsOld.getCurrentTime() + "');" );
	}

	//============================================================================
	public void focusEditor()
	{
		requestFocus();
		exec( "javascript:RE.focus();" );
	}

	//============================================================================
	public void clearFocusEditor()
	{
		exec( "javascript:RE.blurFocus();" );
	}

	//============================================================================
	private String convertHexColorString( int color )
	{
		return String.format( "#%06X", ( 0xFFFFFF & color ) );
	}

	//============================================================================
	protected void exec( final String trigger )
	{
		if( m_IsReady )
		{
			load( trigger );
		}
		else
		{
			postDelayed( new Runnable()
			{
				@Override
				public void run()
				{
					exec( trigger );
				}

			}, 100 );
		}
	}

	//============================================================================
	private void load( String trigger )
	{
		// if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
		// evaluateJavascript(trigger, null);
		// } else {
		loadUrl( trigger );
		// }
	}

	//============================================================================
	protected class EditorWebViewClient extends WebViewClient
	{
		@Override
		public void onPageFinished( WebView view, String url )
		{
			m_IsReady = url.equalsIgnoreCase( SETUP_HTML );
			if( m_LoadListener != null )
			{
				m_LoadListener.onAfterInitialLoad( m_IsReady );
			}
		}

		@Override
		public boolean shouldOverrideUrlLoading( WebView view, String url )
		{
			String decode;
			try
			{
				decode = URLDecoder.decode( url, "UTF-8" );
			}
			catch( UnsupportedEncodingException e )
			{
				// No handling
				return false;
			}

			if( TextUtils.indexOf( url, CALLBACK_SCHEME ) == 0 )
			{
				callback( decode );
				return true;
			}
			else if( TextUtils.indexOf( url, STATE_SCHEME ) == 0 )
			{
				stateCheck( decode );
				return true;
			}

			return super.shouldOverrideUrlLoading( view, url );
		}
	}
}