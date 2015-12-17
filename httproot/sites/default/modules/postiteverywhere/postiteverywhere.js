// $Id: postiteverywhere.js,v 1.1 2010/01/28 12:13:07 confiz Exp $

/**
 * @file
 *   The file contains javascript that is being used in this module
 * @author
 *  Qasim Zeeshan
 */

/**
 * Facebook API function 
 */
function facebookOnLoginReady() {
  $("#fconnect-autoconnect-form").submit();
}

/**
 * We need 'publish_stream' in order to post status messages to the users
 */
function publishStreamPersonal() {
  if(FB.Connect == null) {
    alert("Cannot connect to facebook. Please check your network connection or api keys"); return;}
  FB.Connect.showPermissionDialog("publish_stream",
    function(x) {
      if(x == "publish_stream") //If user clicked "Allow Publishing"
        location.href = "/postiteverywhere/facebook/added";
      },
    true);
}

/**
 * Verify API public key to get connected with the application
 */
function facebookRequireFeatures(apikey, xdpath) {
  FB.Bootstrap.requireFeatures(["Connect","XFBML"], function() {
    FB.Facebook.init(apikey, xdpath);
  });
}

/**
 * On logout from site, logout from facebook. A good security practice
 */
function facebookLogout(url) {
  $(document).ready(function() {
    FB.Bootstrap.requireFeatures(["Connect"], function() {
      FB.Connect.logoutAndRedirect(url);
    });
  });
}
