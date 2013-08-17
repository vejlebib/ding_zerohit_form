/**
* @file
* Inject a form when search returns 0 results.
*
*/
(function ($) {
  $(document).ready(function($) {
    if ($('#ting-result').length != 0) {
      //...wait for the ting search to complete
      var waitForSearchResultTimer = window.setInterval(function() {
        //console.log(Drupal.tingSearch.summary.ting); //uncomment this line for debugging through javascript console

        if (Drupal.tingSearch.summary.ting) {
          clearInterval(waitForSearchResultTimer);
          if (Drupal.tingSearch.summary.ting.count == 0) {
            // Get search string from URL
            var href = window.location.href;
            var searchStr = href.substr(href.lastIndexOf('/') + 1);

            // Load up the zerohit form into the ting-result box
            $('#ting-result').load('/ding/zerohitform/'+searchStr+'?search='+searchStr);
          }
        }
      }, 1000);
    }
  });
})(jQuery);
