
  <div id="footer">
      <!-- start footer section -->

      <img id="footer-header-bar" src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-top-bar.png" />
      <div id="footer-content">
      	<div id="footer-left-column-wrapper">
        	<div id="footer-section-wrapper">
                <div class="footer-column">
                 <div class="footer-section-header">
                 <a href="/portfolio"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-header-portfolio.png" alt="Portfolio" /></a>
                 </div>

                 <ul class="menu">
                    <li class="leaf first"><a href="/portfolio/medium">medium</a></li>
                    <li class="leaf"><a href="/portfolio/technology">technology</a></li>
                    <li class="leaf"><a href="/portfolio/skill">skill</a></li>
                    <li class="leaf"><a href="/portfolio/service">service</a></li>
                    <li class="leaf"><a href="/portfolio/industry">industry</a></li>
                    <li class="leaf last"><a href="/portfolio/client">client</a></li>
                 </ul>
                </div>

                <div class="footer-column">
                 <div class="footer-section-header">
                 <a href="/gallery"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-header-gallery.png" alt="Gallery" /></a>
                 </div>

                 <ul class="menu">
                    <li class="leaf first"><a href="/gallery/photography">photography</a></li>
                    <li class="leaf"><a href="/gallery/illustration">illustration</a></li>
                    <li class="leaf last"><a href="/gallery/painting">painting</a></li>
                 </ul>
                </div>

                
                <div class="footer-column">
                 <div class="footer-section-header">
                 <a href="/knowledge"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-header-knowledge.png" alt="knowledge" /></a>
                 </div>

                 <ul class="menu">
                    <li class="leaf first"><a href="/blogs">blog</a></li>
                    <?php /* ?>
                    <li class="leaf"><a href="/knowledge/articles">articles</a></li>
                    <li class="leaf"><a href="/knowledge/bookmarks">bookmarks</a></li>
                    <li class="leaf last"><a href="/knowledge/code">code</a></li>
                    <?php */ ?>
                 </ul>

                </div>

                <?php /* ?>

                <div class="footer-column">
                 <div class="footer-section-header">
                    <img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-header-services.png" alt="Services" />
                 </div>

                 <ul class="menu">
                    <li class="leaf first"><a href="http://projects.robertfoleyjr.com">projects</a></li>
                    <li class="leaf" last><a href="http://features.robertfoleyjr.com">features</a></li>
                 </ul>
                </div>

                <?php */ ?>

                <div class="footer-column">
                  <div class="footer-section-header">
                     <img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/footer-header-community.png" alt="Community" />
                  </div>

                  <ul class="menu">
                    <li class="leaf first"><a href="http://digg.com/users/zeroplane">digg</a></li>
                    <li class="leaf first"><a href="http://www.linkedin.com/in/robertfoleyjr">linkedin</a></li>
                    <li class="leaf"><a href="http://twitter.com/robertfoleyjr">twitter</a></li>
                    <li class="leaf"><a href="http://www.facebook.com/robertfoleyjr">facebook</a></li>
                    <li class="leaf"><a href="http://delicious.com/robertfoley">delicious</a></li>
                    <li class="leaf last"><a href="/feed/blogs">rss</a></li>
                 </ul>
                </div>
            </div>

            <div id="sub-footer-wrapper">
            	<div id="mini-logo-wrapper"><a href="/"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/logo-small-sanstext.png" width="22" height="30" /></a></div>
                <div id="footer-links">
                <a href="/project-quote-request">Request Quote</a>
                <a href="/about-me">About Me</a>
                </div>
            </div>

        </div>
        <div id="footer-right-column-wrapper">
            <div id="footer-right-column-header">
            <a href="/contact"><img src="/<?php print(drupal_get_path('theme', 'carbon')); ?>/images/looking-advert.png" width="192" height="48" alt="Looking for a solution? Contact Me!" /></a>
               <br /><br />
               <div id="google_translate_element"></div><script>
                function googleTranslateElementInit() {
                 new google.translate.TranslateElement({
                 pageLanguage: 'en'
                 }, 'google_translate_element');
                }
               </script><script src="//translate.google.com/translate_a/element.js?cb=googleTranslateElementInit"></script>
            </div>
            <div id="footer-right-column-footer">
            © Robert Foley Jr 1975-<?php print(date('Y')); ?> Content created and posted on this site is copyright Robert Foley Jr
            </div>
        </div>
      </div>

      <!-- end footer section -->
  </div>
    <!-- /.left-corner, /.right-corner, /#squeeze, /#center -->
  </div>
  <!-- /container -->
</div>
</div>
<!-- /layout -->


  <?php print $closure ?>
  </body>
</html>




