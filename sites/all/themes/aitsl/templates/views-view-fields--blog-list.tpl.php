<?php
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
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>

  <div class="blog-list-row">
    <div class="blog-list-date">
      <div class="blog-list-day"><?php print $fields['created']->content; ?></div>
      <div class="blog-list-month"><?php print $fields['created_1']->content; ?></div>
    </div>
    <div class="blog-list-content">
      <div class="blog-list-header">
        logo
        <div class="blog-list-title"><?php print $fields['title']->content; ?></div>
        <div class="blog-list-author"><?php print $fields['name']->content; ?></div>
      </div>
      <div class="blog-list-body">
        <div class="blog-list-image"><?php print $fields['field_blog_image']->content; ?></div>
        <div class="blog-list-text"><?php print $fields['body']->content; ?></div>
      </div>
      <div class="blog-list-footer">
        <div class="blog-list-comment"><?php print $fields['comment_count']->content; ?> Comments</div>
      </div>
    </div>
  </div>

