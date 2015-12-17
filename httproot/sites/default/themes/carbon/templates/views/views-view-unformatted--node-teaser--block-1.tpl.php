<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php for($i = 0; $i < count($rows); $i++) { ?>

  <div class="slide <?php ($i == 0) ? print('active') : '';?>">
                  <!-- start slide -->
                  <?php print($rows[$i]); ?>
                  <!-- end slide -->
    </div>

<?php }?>
<?php /* foreach ($rows as $id => $row): ?>
    <?php print $row; ?>
<?php endforeach; */ ?>
