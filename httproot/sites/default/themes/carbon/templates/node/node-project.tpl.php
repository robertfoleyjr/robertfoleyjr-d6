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


// build client link
$client_result = db_query("SELECT ua.dst, td.name FROM {url_alias} AS ua, {term_data} AS td WHERE ua.src = 'taxonomy/term/%s' AND td.tid = %d", (string)$node->field_node_company[0]['value'], $node->field_node_company[0]['value']);
$client_row = db_fetch_array($client_result);

if ($node->field_node_client[0]['value'] != 1) {

$employer_result = db_query("SELECT ua.dst, td.name FROM {url_alias} AS ua, {term_data} AS td WHERE ua.src = 'taxonomy/term/%s' AND td.tid = %d", (string)$node->field_node_employer[0]['value'], $node->field_node_employer[0]['value']);
$employer_row = db_fetch_array($employer_result);

}
?>
<pre>
<?php //print_r($node); ?>
</pre>

<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h1><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h1>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>
    
    <div id="project-details-header-wrapper">
        <div id="project-details-row1">
            <h2>Client: <?php print('<a href="/portfolio/client/'.$client_row['dst'].'">'.$client_row['name'].'</a>'); ?></h2>
            <?php if ($node->field_node_client[0]['value'] != 1) { ?>
            <h2>Employer: <?php print('<a href="/portfolio/employer/'.$employer_row['dst'].'">'.$employer_row['name'].'</a>'); ?></h2>
            <?php } ?>
        </div>
        <div id="project-details-row2">
            <div id="project-start-wrapper">
                <label>Start Date</label> <?php (empty($node->field_node_start_date[0]['value']) == FALSE) ? print(date('m/d/Y', $node->field_node_start_date[0]['value'])) : ''; ?>
            </div>
            <div id="project-end-wrapper">
                <label>End Date</label> <?php (empty($node->field_node_end_date[0]['value']) == FALSE) ? print(date('m/d/Y', $node->field_node_end_date[0]['value'])) : ''; ?>
            </div>
            <div id="project-status-wrapper">
                <label>Status</label> <?php print($node->field_node_status[0]['view']); ?>
            </div>
        </div>
        
    </div>

  <div class="content clear-block">
    <?php //print $content ?>
      <?php
      $emptycount = stripos($node->content['body']['#value'], '<br />');
      $emptylength = strlen($node->content['body']['#value']);
      
      if ($emptycount !== FALSE && $emptylength == 7) {
        print('Either this portfolio entry is a "stub" for an active project or I am lagging behind on detailing my projects.<br />Not to worry I will detail this project as soon as possible. In the meantime select a category from the list on the right to view similar projects.<br /><br />Are you impressed with the body of work I have done? How about <a href="/contact">contacting</a> me so I can help you realize your goals?');
      } else {
        print($node->content['body']['#value']);
      }
     ?>
  </div>

  <div class="clear-block">
    <?php /*
      <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>
   <?php */ ?>

      <?php
        $project_images = views_embed_view('node_projects', 'page_9', $node->nid);
        if (count($node->field_project_images) > 0 && empty($node->field_project_images[0]['view']) == FALSE) {
      ?>

      <h3>Images</h3>
      <div id="project-images-wrapper">
      <?php
        print($project_images);
      ?>
      </div>

      <?php
        }
      ?>


      <?php
        if (count($node->field_project_files) > 0 && empty($node->field_project_files[0]['view']) == FALSE) {
      ?>
      <h3>Files</h3>
      <div id="project-files-wrapper">
      <?php foreach($node->field_project_files AS $file_row) { ?>
          <div id="profile-file"><?php print($file_row['view']); ?></div>
      <?php } ?>
      </div>
      <?php } ?>

      
      <?php
        if (count($node->field_project_links) > 0 && empty($node->field_project_links[0]['view']) == FALSE) {
      ?>
      <h3>Links</h3>
      <div id="project-links-wrapper">
          <?php foreach($node->field_project_links AS $link_row) { ?>
          <div id="profile-link"><?php print($link_row['view']); ?></div>
      <?php } ?>
      </div>
      <?php
        }
      ?>

      <?php
        if (count($node->field_project_related) > 0 && empty($node->field_project_related[0]['view']) == FALSE) {
      ?>
      <h3>Related Projects</h3>
      <div id="project-related-wrapper">
          <?php foreach($node->field_project_related AS $project_row) { ?>
          <div id="profile-related-project"><?php print($project_row['view']); ?></div>
      <?php } ?>
      </div>
      <?php
        }
      ?>
      

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
