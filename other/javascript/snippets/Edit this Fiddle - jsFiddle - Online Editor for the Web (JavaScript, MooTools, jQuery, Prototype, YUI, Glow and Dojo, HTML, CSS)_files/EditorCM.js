/*
 Class: MooshellEditor
 Editor using CodeMirror
 http://marijn.haverbeke.nl/codemirror/index.html
 */

var disallowedPlatforms = ['ios', 'android', 'ipod'];

var MooShellEditor = new Class({
	Implements: [Options, Events, Class.Occlude],
	parameter: "Editor",
	options: {
		useCodeMirror: true,
		codeMirrorOptions: {
			lineNumbers: false,
			autoMatchParens: true,
			indentUnit: 4,
			tabMode: 'shift',
			height: ""
		}
	},
	initialize: function(el, options) {
		// switch off CodeMirror for IE
		//if (Browser.Engine.trident) options.useCodeMirror = false;
		this.element = $(el);
		if (this.occlude()) return this.occluded;
		this.setOptions(options);
        var is_disallowed = (disallowedPlatforms.contains(Browser.Platform.name));
		if (this.options.useCodeMirror && CodeMirror.isProbablySupported() && !is_disallowed) {
            // hide textarea
            this.element.hide();
            // prepare settings
			if (!this.options.codeMirrorOptions.stylesheet && this.options.stylesheet) {
				this.options.codeMirrorOptions.stylesheet = this.options.stylesheet.map( function(path) {
					return mediapath + path;
				});
			}
			if (!this.options.codeMirrorOptions.path) {
				this.options.codeMirrorOptions.path = codemirrorpath + 'js/';
			}
            if (!this.options.codeMirrorOptions.content) {
              this.options.codeMirrorOptions.content = this.element.get('value'); 
            }
            // run this after initialization
            if (!this.options.codeMirrorOptions.initCallback) {
              this.options.codeMirrorOptions.initCallback = function(){
                // grab some keys
                this.editor.grabKeys(
                  Layout.routeReservedKey.bind(Layout), 
                  Layout.isReservedKey.bind(Layout)
                );
                if (this.options.focus) {
                  // set current editor
                  Layout.current_editor = this.options.name;
                  // grab focus
                  this.editor.focus();
                }
                // set current editor if user focuses by mouse
                this.editor.win.addEventHandler(this.editor.win, 'focus', function() {
                  Layout.current_editor = this.options.name;
                }.bind(this));
              }.bind(this)
            }
			this.editor = new CodeMirror(this.element.getParent(), this.options.codeMirrorOptions);
		}
		this.editorLabelFX = new Fx.Tween(this.getLabel(), {property: 'opacity', link: 'cancel'});
		this.getWindow().addEvents({
			mouseenter: function() {
				this.editorLabelFX.start(0);
				//this.fullscreen.retrieve('fx').start(0.3);
			}.bind(this),
			mouseleave: function() {
				this.editorLabelFX.start(0.8);
				//this.fullscreen.retrieve('fx').start(0);
			}.bind(this)
		});
		mooshell.addEvents({
			'run': this.b64decode.bind(this)
		});
		Layout.registerEditor(this);
	},
	getEditor: function() {
		return this.editor || this.element;
	},
	getWindow: function() {
		if (!this.window) {
			this.window = this.element.getParent('.window');
		}
		return this.window;
	},
	getLabel: function() {
		return this.getWindow().getElement('.window_label');
	},
	b64decode: function() {
		this.element.set('value', this.before_decode);
	},
    getCode: function() {
      return (this.editor) ? this.editor.getCode() : this.element.get('value');
    },
	updateFromMirror: function() {
		this.before_decode = this.getCode(); 
		this.element.set('value', Base64.encode(this.before_decode));
	},
	clean: function() {
		this.element.set('value','');
		this.cleanEditor();
	},
	cleanEditor: function() {
		if (this.editor) this.editor.setCode('');
	},
	hide: function() {
		this.getWindow().hide();
	},
	show: function() {
		this.getWindow().show();
	},
	setStyle: function(key, value) {
		if (this.editor) return $(this.editor.frame).setStyle(key, value);
		return this.element.setStyle(key, value);
	},
	setStyles: function(options) {
		if (this.editor) return $(this.editor.frame).setStyles(options);
		return this.element.setStyles(options);
	},
	setWidth: function(width) {
		this.getWindow().setStyle('width',width);
		//this.setStyle('width', width);
	},
	setHeight: function(height) {
		this.getWindow().setStyle('height',height);
		//this.setStyle('height', height);
	},
	getPosition: function() {
		if (this.editor) return $(this.editor.frame).getPosition();
		return this.element.getPosition();
	}
});


/*
 * JS specific settings
 */
MooShellEditor.JS = new Class({
	Extends: MooShellEditor,
	options: {
		name: 'js',
        focus: true,
		useCodeMirror: true,
		flexibleHeight: true,
		stylesheet: [
			"css/codemirror/style.css",
			"css/codemirror/jscolors.css"
		],
		codeMirrorOptions: {
			iframeClass: 'js',
			parserfile: ["tokenizejavascript.js", "parsejavascript.js"]
		}
	},
	initialize: function(el,options) {
		this.setOptions(options);
		this.parent(el,this.options);
	}
});



/*
 * CSS specific settings
 */
MooShellEditor.CSS = new Class({
	Extends: MooShellEditor,
	options: {
		name: 'css',
		useCodeMirror: true,
		stylesheet: [
			"css/codemirror/style.css",
			"css/codemirror/csscolors.css"
		],
		codeMirrorOptions: {
			iframeClass: 'css',
			parserfile: ["parsecss.js"]
		}
	},
	initialize: function(el,options) {
		this.setOptions(options);
		this.parent(el,this.options);
	}
});


/*
 * HTML specific settings
 */
MooShellEditor.HTML = new Class({
	Extends: MooShellEditor,
	options: {
		name: 'html',
		useCodeMirror: true,
		stylesheet: [
			"css/codemirror/style.css",
			"css/codemirror/xmlcolors.css"
		],
		codeMirrorOptions: {
			iframeClass: 'html',
			parserfile: ["parsexml.js"]
		}
	},
	initialize: function(el,options) {
		this.setOptions(options);
		this.parent(el,this.options);
	}
});
