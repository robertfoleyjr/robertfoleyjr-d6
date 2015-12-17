<?php include_once(drupal_get_path('theme', 'carbon').'/includes/header.php'); ?>

    <div id="slideshow-wrapper">
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
          <?php if ($title): print '<h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h1>'; endif; ?>
          <?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
          <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
          <?php if ($show_messages && $messages): print $messages; endif; ?>
          <?php print $help; ?>
            <div class="clear-block">
              <!-- content section -->
                <?php print $breadcrumb; ?>
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