package net.sourceforge.ufoai.ui.syntaxcoloring;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.xtext.ui.editor.syntaxcoloring.DefaultHighlightingConfiguration;
import org.eclipse.xtext.ui.editor.syntaxcoloring.IHighlightingConfigurationAcceptor;
import org.eclipse.xtext.ui.editor.utils.TextStyle;

public class HighlightingConfiguration extends DefaultHighlightingConfiguration {
	public static final String RULE_PROPERTY_TYPE = "ufoProperty"; //$NON-NLS-1$
	public static final String RULE_PROPERTY_VALUE = "ufoValue"; //$NON-NLS-1$
	public static final String RULE_NAMED_BLOCK = "ufoBlock"; //$NON-NLS-1$

	@Override
	public void configure(IHighlightingConfigurationAcceptor acceptor) {
		super.configure(acceptor);
		acceptor.acceptDefaultHighlighting(RULE_NAMED_BLOCK, "Named Block", blockTextStyle());
		acceptor.acceptDefaultHighlighting(RULE_PROPERTY_TYPE, "Property", propertyTextStyle());
		acceptor.acceptDefaultHighlighting(RULE_PROPERTY_VALUE, "Value", valueTextStyle());
	}

	private TextStyle propertyTextStyle() {
		TextStyle textStyle = defaultTextStyle().copy();
		textStyle.setColor(new RGB(128, 0, 0));
		textStyle.setStyle(SWT.BOLD);
		return textStyle;
	}

	private TextStyle valueTextStyle() {
		TextStyle textStyle = defaultTextStyle().copy();
		textStyle.setColor(new RGB(0, 0, 255));
		// textStyle.setStyle(SWT.BOLD);
		return textStyle;
	}

	private TextStyle blockTextStyle() {
		TextStyle textStyle = defaultTextStyle().copy();
		textStyle.setColor(new RGB(128, 0, 0));
		textStyle.setStyle(SWT.BOLD | SWT.ITALIC);
		return textStyle;
	}
}
