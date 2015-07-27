javascript:(function(){
	$(".follow-more-than-two a").each(function(){
		$("<span>").text(
			$(this).attr("original-title")
		).appendTo(
		$(this).parent().css("display","block")
		).after($("<br>"));
	});
})();
