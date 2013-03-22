<?php

/**
  * @file
  * Customize confirmation screen after successful submission of the zero-hit form.
  *
  * Available variables:
  * - $webform_node: object containing data on the zerohit webform node
  * - $submission: object containing data on this submission
  * - $confirmation_message: The confirmation message input by the zerohit webform author.
  * - $user_description: Description as input in user's submission.
  * - $user_name: User's name as input in user's submission.
  */
?>

<div class="zerohit-confirmation">
  <?php if ($confirmation_message): ?>
    <?php print $confirmation_message ?>
  <?php else: ?>
    <p><?php print t('Thank you, your submission has been received.'); ?></p>
  <?php endif; ?>
</div>