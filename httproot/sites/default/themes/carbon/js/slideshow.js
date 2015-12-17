

var slideshow = {

  number_of_slides: 0,
  slides: new Array(),
  active_slide: null,
  slideshow_interval: 10000,
  
  init: function() {

    slideshow.number_of_slides = $('.slideshow div.slide').length;

    if(slideshow.number_of_slides == 0) {
      return;
    }

    $('#slideshow-control-list').append('<ul id="slideshow-control-number-list"></ul>');

    var slide_number = 1;
    $('.slideshow div.slide').each(function() {

      if(slide_number == 1) {
        $('#slideshow-control-number-list').append('<li class="number first active"></li>');
      } else if(slide_number == slideshow.number_of_slides) {
        $('#slideshow-control-number-list').append('<li class="number last"></li>');
      } else {
        $('#slideshow-control-number-list').append('<li class="number"></li>');
      }

      var slide = slideshow.slides[slide_number] = $(this);
      slide.number = slide_number;
      slideshow.slides[slide_number]['numberObj'] = $('#slideshow-control-number-list li.number').eq(slide_number-1);

      slideshow.slides[slide_number]['numberObj'].bind('click', function() {
        slideshow.stopSlideShow();
        slideshow.showSlide(slide);
      });

      slide_number++;
    });

    $('#slideshow-previous').bind('click', function(){
      slideshow.showPrevious();
    });
    $('#slideshow-next').bind('click', function(){
      slideshow.showNext();
    });

    slideshow.active_slide = slideshow.slides[1];
    $(slideshow.active_slide).addClass('active');
    slideshow.startSlideShow();
  },
  showSlide: function(slide) {
    slideshow.hideCurrentSlide();
    $(slide).fadeIn();
    /*
    if ($.browser.msie) {
      $(slide).find("img[src$=.png]").each(function() {
        var bgIMG = $(slide).attr('src');
        $(slide).get(0).runtimeStyle.filter = 'progid:DXImageTransform.Microsoft.AlphaImageLoader' + '(src=\'' + bgIMG + '\', sizingMethod=\'scale\');';
      })
    } 
    */
    $(slide).addClass('active');
    $(slide.numberObj).addClass('active');
    
    slideshow.active_slide = slide;
  },
  hideCurrentSlide: function() {
    $(slideshow.active_slide).removeClass('active');
    $(slideshow.active_slide.numberObj).removeClass('active');
    $(slideshow.active_slide).removeAttr('style');
    $(slideshow.active_slide).hide();
  },
  showNext: function() {
    var next_index = (slideshow.active_slide.number+1 > slideshow.number_of_slides) ? 1 : slideshow.active_slide.number+1;
    slideshow.showSlide(slideshow.slides[next_index]);
  },
  showPrevious: function() {
    var next_index = (slideshow.active_slide.number-1 > 1) ? slideshow.active_slide.number-1 : 1;
    slideshow.showSlide(slideshow.slides[next_index]);
  },
  selectNext: function() {
    var next_index = (slideshow.active_slide.number+1 > slideshow.number_of_slides) ? 1 : slideshow.active_slide.number+1;
    slideshow.stopSlideShow();
    slideshow.showSlide(slideshow.slides[next_index]);
  },
  selectPrevious: function() {
    var next_index = (slideshow.active_slide.number-1 > 0) ? slideshow.active_slide.number-1 : slideshow.number_of_slides;
    slideshow.stopSlideShow();
    slideshow.showSlide(slideshow.slides[next_index]);
  },
  startSlideShow: function() {
    slideshow.interval = setInterval(slideshow.showNext, slideshow.slideshow_interval);
  },
  stopSlideShow: function() {
    clearInterval(slideshow.interval);
  }
}