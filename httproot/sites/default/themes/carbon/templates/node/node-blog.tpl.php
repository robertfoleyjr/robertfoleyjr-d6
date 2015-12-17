<?php
// $Id: node.tpl.php,v 1.4 2008/01/25 21:21:44 goba Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> node-type-blog">
<div class="view view-blog-index view-id-blog_index view-display-id-page_2 view-dom-id-2">
<?php print $picture ?>

<?php if ($page == 0): ?>
  <h1><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h1>
<?php endif; ?>

  <span class="field-content post-date">
    <h3><?php print(date('l, F j, Y',strtotime($node->field_blog_postdate[0]['value']))); ?></h3>
  </span>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>

  <div class="content clear-block">
    <?php print $content ?>
  </div>

  <div class="clear-block">
    <div class="author">
        Posted by:  <?php print($node->field_blog_author_user[0]['view']); ?> at <?php print(date('g:i A',strtotime($node->field_blog_postdate[0]['value']))); ?>
    </div>
  </div>


  <div class="clear-block">

  <?php
   if (count($node->field_blog_tags) > 0 && empty($node->field_blog_tags[0]['value']) == FALSE) { ?>
    <div class="meta">
      <span class="field-content">
    <?php
    $taglist_string = '';

     foreach($node->field_blog_tags AS $tag_item) {
             $taglist_string .= $tag_item['value'].",";
      }

    $taglist_string = substr($taglist_string, 0, -1);


      $taglist_links = "";
      $tab_result = db_query("SELECT td.tid, td.name, (SELECT ua.dst FROM {url_alias} AS ua WHERE ua.src LIKE CONCAT('taxonomy/term/',td.tid)) AS `alias` FROM {term_data} AS td  WHERE td.tid in (%s)", $taglist_string);
      while($term_row_item = db_fetch_object($tab_result)) {
          $taglist_links .= '<a href="/blogs/search/tag/' . $term_row_item->alias . '">' . $term_row_item->name . '</a> ';
      }
      print('Tags: '.$taglist_links);

    ?>
      </span>
    </div>
  <?php } ?>

      <iframe src="http://www.facebook.com/plugins/like.php?href=<?php print("http" . ((!empty($_SERVER['HTTPS'])) ? "s" : "") . "://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']); ?>&amp;layout=standard&amp;show_faces=true&amp;width=450&amp;action=like&amp;font=trebuchet+ms&amp;colorscheme=dark&amp;height=80" scrolling="no" frameborder="0" style="border:none; overflow:hidden; width:450px; height:25px; margin-top: 10px;" allowTransparency="true"></iframe>


    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>
</div>
</div>