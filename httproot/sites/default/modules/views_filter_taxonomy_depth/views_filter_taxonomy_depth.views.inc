<?php

/**
 * Implementation of hook_views_handlers().
 */
function views_filter_taxonomy_depth_views_handlers() {
	return array(
		'handlers' => array(
			'views_filter_taxonomy_depth_handler_filter_taxonomy_depth' => array(
				'parent' => 'views_handler_filter_term_node_tid'
			),
		)
	);
}

function views_filter_taxonomy_depth_views_data_alter(&$data) {
	$data['node']['term_node_tid_depth']['filter'] = array(
		'handler' => 'views_filter_taxonomy_depth_handler_filter_taxonomy_depth',
	);
}

