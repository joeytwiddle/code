var filter = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="0%" height="0%"><filter id="EdgeDetect"><feConvolveMatrix order="3 3" preserveAlpha="true" kernelMatrix="-1 -1 -1 -1 8 -1 -1 -1 -1"/></filter></svg>';

$("head").append(filter);

$("head").append('<style>.ed {-webkit-filter: url(#EdgeDetect) grayscale(100%) invert(100%);}</style>');

$("html").addClass("ed");
