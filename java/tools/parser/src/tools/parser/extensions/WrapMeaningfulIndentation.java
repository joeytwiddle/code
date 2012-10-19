package tools.parser.extensions;

import tools.parser.PreProcessor;


public class WrapMeaningfulIndentation implements PreProcessor {

	private String openSymbol;
	private String closeSymbol;

	public WrapMeaningfulIndentation(String openSymbol, String closeSymbol) {
	   this.openSymbol = openSymbol;
	   this.closeSymbol = closeSymbol;
   }

}
