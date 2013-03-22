<?php

/**
  * @file
  * Customize confirmation screen after successful submission of the zero-hit form.
  *
  * Available variables:
  * - $confirmation_message: The confirmation message input by the zerohit webform author.
  * - $sid: The unique submission ID of this submission.
  */
?>
 
<div class="webform-confirmation">
  <?php if ($confirmation_message): ?>
    <?php print $confirmation_message ?>
  <?php else: ?>
    <p><?php print t('ZEROHIT!!! Thank you, your submission has been received.'); ?></p>
  <?php endif; ?>
  
  sid: <?php print $sid; ?>
</div>