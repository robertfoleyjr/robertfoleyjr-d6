; $Id: README.txt,v 1.1.2.4 2010/05/05 11:35:04 confiz Exp $

CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Contact

INTRODUCTION
------------

PostItEveryWhere is a module that will help users to post the content to Facebook,
Twitter, LinkedIn and FriendFeed with just one click. The users don't have to
Authorize every time, the authorization process is required only once and then
users post to the added Social Networks as long as their settings are saved.
The module communicates with the social networks by their applications. It uses
Facebook Connect and OAuth Libraries for users' authentication. At no place, the
module reads or saves the credentials of any social networks
related to a user. The module is only usable to Authenticated users

REQUIREMENTS
------------
PHP 5.2 or higher.
Drupal 6.x.
OAuth PHP Library: http://github.com/abraham/twitteroauth/raw/master/twitteroauth/OAuth.php
Facebook PHP Library: http://drupal.org/files/issues/facebook_client.zip
Facebook API key: http://www.facebook.com/developers/
Twitter API Key: https://twitter.com/apps/new
LinkedIn API Key: https://www.linkedin.com/secure/developer?newapp=
FriendFeed API Key: http://friendfeed.com/api/register

INSTALLATION
------------

Following steps are needed to install PostItEveryWhere:

IMPORTANT: This module can only be used by authenticated users so please be careful
while assigning rights to anonymous users.

1. Enable Curl
	cUrl should be enabled. Open php.ini and change the statement
  ';extension=php_curl.dll' to 'extension=php_curl.dll' (quotes for clarity)

2. Get facebook API key:
	i.	Create a facebook application (http://www.facebook.com/developers/apps.php),
      this application will be used to connect users of your site with facebook.
      Connect URL should be [YOUR DOMAIN] e.g. 'www.example.com'
	ii.	Add [xmlns:fb="http://www.facebook.com/2008/fbml"] the HTML tag of your
      theme's page.tpl file to enable FBML in your page
	iii. Get facebook library from http://svn.facebook.com/svnroot/platform/clients/packages/facebook-platform.tar.gz
       and place the content of php folder in 'facebook-client' folder after
       decompressing the package.

3. Get Twitter registered
	i.	Create and application on twitter https://twitter.com/apps/new
		-> Application website should be [YOUR Domain] e.g. 'www.example.com'
       (Twitter don�t allow domain names like 'http://drupal6' so make sure
       that your site name is similar to a domain e.g. �http://drupal6.org�)
		-> Application type 'Browser'
		-> Call back URL should be '[YOUR Domain]/postiteverywhere/addnetworks/twitter/added'
       e.g. http://example.com/postiteverywhere/twitter/added
		-> Default Access Type 'Read/Write'
	ii.	Download OAuth API from http://github.com/abraham/twitteroauth/raw/master/twitteroauth/OAuth.php
      and place it in the �api� directory.

4. LinkedIn
	i.	Create application https://www.linkedin.com/secure/developer?newapp=
	ii.	Call back URL should be '[YOUR Domain]/postiteverywhere/addnetworks/linkedin/added'
      e.g. http://example.com/postiteverywhere/linkedin/added
	iii.	Make sure that OAuth API is available in the 'api' directory

5. FriendFeed
	i.	Create application http://friendfeed.com/api/register
	ii.	Call back URL should be '[YOUR Domain]/postiteverywhere/addnetworks/friendfeed/added'
      e.g. http://example.com/postiteverywhere/friendfeed/added
	iii.	Make sure that OAuth API is available in the 'api' directory

6. After getting all the API keys, go to Settings page(admin/settings/postiteverywhere)
   and save the keys, you can also perform other settings as required

CONTACT
-------
Confiz Solutions: http://www.confiz.com
Developer Contact: qasim.zeeshan@confiz.com