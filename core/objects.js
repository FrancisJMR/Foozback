var FileManager = Class.create();
FileManager.prototype = {
	initialize: function(elem_id) {
		this.elem_id    = elem_id;
		this.file_name  = '';
		this.action     = null; //current actions: delete
		this.call_back  = 'core/objects_cb.php';
	},

	setAction: function(action) {
		this.action = action;
	},

	getUrlParams: function(param_names, param_values) {
		var i=0;
		var param_list = this.call_back + '?action' + '=' + this.action;
		for(i=0; i<param_names.length; i++){
			param_list += '&' + param_names[i] + '=' + param_values[i];
		}
		return param_list;
	},

	update: function(content) {
		var element = $(this.elem_id);
		if(this.action === 'delete') {
			element.parentNode.removeChild(element);
//			element.innerHTML = content;
		}
	},

	request: function(param_names, param_values) {
		self = this;
		var result = new Ajax.Request( self.call_back, {
			parameters: self.getUrlParams(param_names, param_values),
			onSuccess: function(transport) {
				var content = transport.responseText;
				//alert(content);
				self.update(content);
			},
			onFailure: function(e) {
				 alert(e);
			}
		});

	},

	deleteFile: function(file_id, file_name) { 
		// set what update needs to know
		this.elem_id = file_id;
		this.action  = 'delete';

		// setup the post params
		var param_names  = new Array();
		var param_values = new Array();
		param_names.push('file_name');
		param_values.push(file_name);

		this.request(param_names, param_values);
	}

};

var FormManager = Class.create();
FormManager.prototype = {
	initialize: function(elem_id, callback_filename) {
		this.elem_id     = elem_id;
		this.action      = null; //current actions: delete
		this.call_back   = callback_filename;
		this.sections_id = new Array(); //all the sections for this SF
	},
	
	loadSections: function() {
		var form = $(this.elem_id);
		var section = form.down();
		var i = 0;
		while(section != undefined) {
			this.sections_id.push(section.id);
			if (i > 0) section.hide();
			section = section.next();
			i++;
		}
	},
	
	nextSection: function(cur_section_id) {
		var form = $(this.elem_id);
		var i;
		var section;
		for (i=0; i<this.sections_id.length;i++) {
			section = $(this.sections_id[i]);
			if (this.sections_id[i] == cur_section_id)	{
				section.hide();
				var next_section = section.next();
				next_section.show();
				i++;
			} else {
				section.hide();
			}
		}
	},
	
	prevSection: function(cur_section_id) {
		var form = $(this.elem_id);
		var i;
		var section;
		for (i=this.sections_id.length-1; i>-1;i--) {
			section = $(this.sections_id[i]);
			if (this.sections_id[i] == cur_section_id)	{
				section.hide();
				var prev_section = section.previous();
				prev_section.show();
				i--;
			} else {
				section.hide();
			}
		}
	},
	
	saveForm: function() {
		var form = $(this.elem_id);
		this.request(form.serialize());
	},	

	request: function(params) {
		var self = this;
		var result = new Ajax.Request( self.call_back, {
			parameters: params,
			onSuccess: function(transport) {
				var content = transport.responseText;
				//alert(content);
				self.update(content);
			},
			onFailure: function(e) {
				alert('Failure: '+ e);
			}
		});
	},
	
	update: function(content) {
		var form = $(this.elem_id);
		var i;
		var section;
		for (i=0; i<this.sections_id.length; i++) {
			section = $(this.sections_id[i]);
			section.hide();
		}
		
		var p = document.createElement('p');		
		p.innerHTML = content;
		form.appendChild(p);
		/*
		previous_p_siblings = p.previousSiblings();
		for (i=0; i<previous_p_siblings.length; i++) {
			sibling = $(previous_p_siblings[i]);
			sibling.hide();
		}
		*/

	}

};


var MenuManager = Class.create();
MenuManager.prototype = {
	initialize: function(elem_id, target_div) {
		this.elem_id    = elem_id;
		this.action     = null; //current actions: delete
		this.call_back  = 'core/objects_cb.php';
		this.target_div = target_div;
	},
	
	// load the post data from the
	// action (link) of the menu item.
	loadPane: function(js_apps) {
		var params = js_apps;
//		var params = $H(js_apps).toJSON();
		alert(params);
		this.request(params);
	},
	
	doAlert: function() {
		alert('this works!');
	},
	
	request: function(params) {
		var self = this;
		var result = new Ajax.Request( self.call_back, {
			parameters: params,
			onSuccess: function(transport) {
				var content = transport.responseText;
				//alert(content);
				self.update(content);
			},
			onFailure: function(e) {
				alert('Failure: '+ e);
			}
		});
	},
	
	update: function(content) {
		var pane = $(this.target_div);
		pane.innerHTML = content;
	}
};