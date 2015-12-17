<?php
// $Id: page.tpl.php,v 1.18.2.1 2009/04/30 00:13:31 goba Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <title><?php print $head_title ?></title>
    <link rel="apple-touch-icon" href="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/apple-touch-icon.png" type="image/png" /> 
    <?php print $styles ?>
    <?php print $scripts ?>
    <script type="text/javascript">
    $(document).ready(function(){
        slideshow.init();
    });
    </script>
    <!--[if lt IE 7]>
      <?php print phptemplate_get_ie_styles(); ?>
    <![endif]-->
    <!--[if lt IE 9]>
      <script type="text/javascript" src="/<?php print drupal_get_path('theme', 'carbon') ?>/js/jquery.curvycorners.source.js"></script>
  <![endif]-->
  </head>
  <body<?php print phptemplate_body_class($left, $right); ?>>
  <?php if (!empty($admin) && $user->uid != 0) print $admin; ?>
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

    <div id="slideshow-wrapper">
        <div id="header-logo">
            <a href="/" style="height: 75px; width: 275px; display: block;"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/logo-large-white.png" title="Robert Foley Jr - Business and Technology Solutions" width="275" height="75" /></a>
        </div>

        <div id="slideshow-footer">
            <img id="footer-header-bar" src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-top-bar.png" />
        </div>

        <div class="slideshow <?php print('slideshow-bg-0'.$randnum = rand(1,3)); ?>">
          <div class="slideshow-inner">

              <?php print(views_embed_view('node_teaser', 'block_1')); ?>

            <div id="slideshow-controls">
              <div id="slideshow-previous"></div>
              <div id="slideshow-control-list"></div>
              <div id="slideshow-next"></div>
            </div>
          </div>
        </div>

        

    </div>

    <div id="content-wrapper">

 <?php if ($left): ?>
    <div id="sidebar-left" class="sidebar">
     <!-- start left row -->
      <?php print $left ?>
      <!-- end left row -->
    </div>
  <?php endif; ?>


    <div id="center">
      <div id="squeeze">
        <div class="right-corner">
          <div class="left-corner">

          <?php if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>
          <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php //if ($title): print '<h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h1>'; endif; ?>
          <?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
          <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
          <?php if ($show_messages && $messages): print $messages; endif; ?>
          <?php print $help; ?>
            <div class="clear-block">
              <!-- content section -->
                <?php //print $breadcrumb; ?>
                <?php print $content ?>

              <!-- end content section -->
            </div>
          </div>
        </div>
      </div>
    </div>

    <?php if ($right): ?>
        <div id="sidebar-right" class="sidebar">
          <?php print $right ?>
        </div>
      <?php endif; ?>

    </div>

<?php include_once(drupal_get_path('theme', 'carbon').'/includes/footer.php'); ?>
