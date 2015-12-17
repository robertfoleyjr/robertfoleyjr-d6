<?php


$term_id = 0;
$term_name = '';
$term_desc = '';
$term_img = '';

if (arg(2)) {
    $term_item = db_fetch_array(db_query('SELECT td.tid, td.name, td.description, ti.path FROM {term_data} AS td LEFT JOIN {term_image} AS ti ON ti.tid = td.tid WHERE td.tid = (SELECT substring(pa.src, 15) AS tid FROM {url_alias} AS pa WHERE pa.dst = "%s")', arg(2)));

    $term_id = $term_item['tid'];
    $term_name = $term_item['name'];
    $term_desc = $term_item['description'];
    $term_img = $term_item['path'];

}

$title = 'Portfolio by Service - '.$term_name;
$breadcrumb = '<div class="breadcrumb"><a id="custom-breadcrumbs-home" href="/">Home</a> » <a class="custom-breadcrumbs-item-1" href="/portfolio">Portfolio</a> » <a class="custom-breadcrumbs-item-2" href="/portfolio/service">by Service</a> » '.$term_name.'</div>';
?>
<?php include_once(drupal_get_path('theme', 'carbon').'/includes/header.php'); ?>

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

              <div id="taxonomy-term-wrapper">
                  <div id="taxonomy-term-img-wrapper">
                      <?php print(taxonomy_image_display($term_id)); ?>
                  </div>
                  <div id="taxonomy-term-description-wrapper">
                      <div id="taxonomy-term-title"><?php print($term_name); ?></div>
                      <div id="taxonomy-term-desc"><?php print($term_desc); ?></div>
                  </div>
              </div>

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