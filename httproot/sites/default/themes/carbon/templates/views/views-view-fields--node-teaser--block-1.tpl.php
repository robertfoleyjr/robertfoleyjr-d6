<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

  $alias_result = db_fetch_array(db_query("SELECT dst FROM {url_alias} WHERE src = CONCAT('node/', %d)", $row->nid));
  $alias = $alias_result['dst'];
  
?>
<div class="teaser-item-main-wrapper">
<?php foreach ($fields as $id => $field): ?>
<?php if ($id == 'title') { ?>
    <div class="teaser-item-column-one-wrapper">
    <div class="teaser-item-title"><a href="/<?php print($alias); ?>"><h2><?php print($field->content); ?></h2></a></div>
<?php }?>

<?php if ($id == 'field_node_company_value') { ?>
    <div class="teaser-item-subtitle"><h3><?php print($field->content); ?></h3></div>
<?php } ?>

<?php if ($id == 'field_node_teaser_desc_value') { ?>
    <div class="teaser-item-desc"><?php print($field->content); ?></div>
<?php } ?>

<?php if ($id == 'view_node') { ?>
    <div class="teaser-item-more"><?php print($field->content); ?></div>
    </div>
<?php } ?>

<?php if ($id == 'field_node_teaser_image_fid') { ?>
    <div class="teaser-item-column-two-wrapper">
                      <div class="teaser-item-image">
                          <a href="/<?php print($alias); ?>"><?php print($field->content); ?></a>
                      </div>
                  </div>
<?php } ?>
    <?php /*?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>

  <<?php print $field->inline_html;?> class="views-field-<?php print $field->class; ?>">
    <?php if ($field->label): ?>
      <label class="views-label-<?php print $field->class; ?>">
        <?php print $field->label; ?>:
      </label>
    <?php endif; ?>
      <?php
      // $field->element_type is either SPAN or DIV depending upon whether or not
      // the field is a 'block' element type or 'inline' element type.
      ?>
      <<?php print $field->element_type; ?> class="field-content"><?php print $field->content; ?></<?php print $field->element_type; ?>>
  </<?php print $field->inline_html; */?>
<?php endforeach; ?>
</div>

