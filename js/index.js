$(document).ready(function() {
  $('.about, .talksworkshops, .projectsHacks .coffeetalk, .press').css({'visibility':'hidden'});

  $(window).scroll(function() { 
    var height = $(window).scrollTop();

    if (height > 190) {
      $('.about').css({"visibility": "visible"}).addClass('animated fadeIn');
      $('.press').css({"visibility": "visible"}).addClass('animated fadeIn');
    }

    if (height > 1190) { 
      $('.projectsHacks').css({"visibility": "visible"}).addClass('animated fadeIn');
      $('.talksworkshops').css({"visibility": "visible"}).addClass('animated fadeIn');
    }

    $('.press').css({"visibility": "visible"}).addClass('animated fadeIn');
    
    $('.coffeetalk').css({"visibility": "visible"}).addClass('animated fadeIn');

  });

  /* Main button clicks */
  $('body').on('click', '#about', function() {
    $('html, body').animate({
          scrollTop: $(".about").offset().top
    }, 500);
    return false;
  });

  $('body').on('click', '#coffeetalk', function() {
    $('html, body').animate({
        scrollTop: $(".coffeetalk").offset().top
    }, 500);
    return false;
  });
  $('body').on('click', '#projectsHacks', function() {
    $('html, body').animate({
        scrollTop: $(".projectsHacks").offset().top
    }, 500);
    return false;
  });

  $('body').on('click', '#talksworkshops', function() {
    $('html, body').animate({
        scrollTop: $(".talksworkshops").offset().top
    }, 500);
    return false;
  });

  $('body').on('click', '#press', function() {
    $('html, body').animate({
        scrollTop: $(".press").offset().top
    }, 500);
    return false;
  });

});