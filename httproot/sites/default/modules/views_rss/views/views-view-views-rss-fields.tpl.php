<?php // $Id$ ?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>

<rss version="2.0" <?php print $namespaces ?>>
  <channel>
    <title><?php print $viewtitle; ?></title>
    <description><?php print $description; ?></description>
    <link><?php print $link ?></link>
    <?php print $rows ?>
  </channel>
</rss>
