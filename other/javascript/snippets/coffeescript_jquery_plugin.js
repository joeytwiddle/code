$.fn.extend
  myplugin: (options) ->
    self = $.fn.myplugin
    opts = $.extend {}, self.default_options, options
    $(this).each (i, el) ->
      self.init el, opts
      self.log el if opts.log

$.extend $.fn.myplugin,
  default_options:
    color: 'red'
    log: true
  
  init: (el, opts) ->
    this.color el, opts
  
  color: (el, opts) ->
    $(el).css('color', opts.color)
  
  log: (msg) ->
    console.log msg
