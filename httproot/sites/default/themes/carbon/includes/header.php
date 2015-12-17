<?php
// $Id: page.tpl.php,v 1.18.2.1 2009/04/30 00:13:31 goba Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <link rel="apple-touch-icon" href="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/apple-touch-icon.png" type="image/png" /> 
    <title><?php print $head_title ?></title>
    <?php print $styles ?>
    <?php print $scripts ?>
    <!--[if lt IE 7]>
      <?php print phptemplate_get_ie_styles(); ?>
    <![endif]-->
  </head>
  <body<?php print phptemplate_body_class($left, $right); ?>>
  <?php if (!empty($admin) && $user->uid != 0) print $admin; ?>
  <!--[if lt IE 9]>
      <script type="text/javascript" src="/<?php print drupal_get_path('theme', 'carbon') ?>/js/jquery.curvycorners.source.js"></script>
  <![endif]-->
  <!--[if lt IE 8]><script src="/ie6/warning.js"></script><script>window.onload=function(){e("/ie6/")}</script><![endif]-->



<!-- Layout -->
<div id="mainheader-bg-wrapper"></div>
<div id="mainwrapper">
<div id="header-region" class="clear-block">

    <div id="header-left-wrapper">
    <div id="header-leftmenu">
      <ul id="top-menu" class="menu">
      	<li class="leaf first"><a href="/">Home</a></li>
        <li class="leaf"><a href="/blogs">Blog</a></li>
        <li class="leaf"><a href="/about-me">About</a></li>
        <li class="leaf last"><a href="/contact">Contact</a></li>
      </ul>
      </div>
    <div id="header-splitter-wrapper">
      <img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/top-menu-splitter.png" width="3" height="19" />
    </div>
    
    <div id="header-leftmenu2">
      <ul id="top-menu" class="menu">
      	<li class="leaf first"><a href="http://projects.robertfoleyjr.com" target="_blank" ><img id="projects-lock-icon" src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/projects-icon.png" width="10" height="12" alt="lock icon" /> Projects</a></li>
        <li class="leaf last feature-menu-item"><a href="http://robertfoley.timetask.com/"><img id="projects-lock-icon" src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/projects-icon.png" width="10" height="12" alt="lock icon" /> Consultants</a></li>
      </ul>
    </div>
     
    </div>


      <div id="header-rightmenu">
      	<ul id="link-menu" class="menu">
      	<li class="leaf first"><a href="http://digg.com/users/zeroplane"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/digg.png" width="16" height="16" alt="digg" /></a></li>
        <li class="leaf"><a href="http://www.linkedin.com/in/robertfoleyjr"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/linkedin.png" width="16" height="16" alt="LinkedIn" /></a></li>
        <li class="leaf"><a href="http://twitter.com/robertfoleyjr"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/twitter.png" width="16" height="16" alt="Twitter" /></a></li>
        <li class="leaf"><a href="http://www.facebook.com/robertfoleyjr"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/facebook.png" width="16" height="16" alt="Facebook" /></a></li>
        <li class="leaf"><a href="http://delicious.com/robertfoley"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/delicious.png" width="16" height="16" alt="delicious" /></a></li>
        <li class="leaf last"><a href="/feed/blogs"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/rss.png" width="16" height="16" alt="rss" /></a></li>
      </ul>
      </div>

</div>
<div id="wrapper">
  <div id="container" class="clear-block">
    <div id="header">
      <!-- header section start -->
      	<div id="primary-menu-wrapper">
            <div id="search-wrapper">
            	<?php if ($search_box): ?><div class="block block-theme"><?php print $search_box ?></div><?php endif; ?>
            </div>

            <div id="primary-menu">
            	<ul class="menu primary-links">
                    <li class="leaf first<?php if (stripos($_REQUEST['q'], 'portfolio') !== FALSE) { print(' active'); } ?>"><a href="/portfolio" class="primary-menu-portfolio"><span>Portfolio</span></a></li>
                    <li class="leaf<?php if (stripos($_REQUEST['q'], 'gallery') !== FALSE) { print(' active'); } ?>"><a href="/gallery" class="primary-menu-gallery"><span>gallery</span></a></li>
                    <li class="leaf<?php if (stripos($_REQUEST['q'], 'knowledge') !== FALSE) { print(' active'); } ?>"><a href="/knowledge" class="primary-menu-knowledge"><span>knowledge</span></a></li>
                    <?php /*
                    <li class="leaf last <?php if (stripos($_REQUEST['q'], 'playground') !== FALSE) { print(' active'); } ?>"><a href="/playground" class="primary-menu-playground"><span>playground</span></a></li>
                     <?php */ ?>
                  </ul>
            </div>
        </div>
      <!-- header section end -->
    </div>
    <!-- /header -->

    <div id="header-sub">
        <div id="header-logo">
            <a href="/"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/logo-large-white.png" title="Robert Foley Jr - Business and Technology Solutions" /></a>
        </div>
    </div>
