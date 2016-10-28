/*
 * index.js
 * Clarifai Basic Application demo code
 */
 /*
 * index.js
 * Clarifai Basic Application demo code
 */
(function ($, Clarifai) {
	$(document).ready(function () {
		initialize()
	})

	// Finding a bunch of elements in the DOM
	var app = $(".app")
	var imageInput = $("#imageUrl");
	var submitButton = $("#submitBtn");
	var image = $("#image");
	var tagsContainer = $(".tags-container");
	var tags = $(".tags");
	var imgUrl = $(".imageUrl");
	var imgUrl2 = $(".formControl");

	submitButton.on("click", function (event) {
		// getting the input from the image
		var url = imageInput.val()
		tagsContainer.hide()

		// You can ignore this part
		// Set's the url of the imae preview on click
		image.attr("src", url)
		jQuery(".imageUrl").focus(function() {
			$(this).val("");
		});


		// Requesting Clarifai to getTagsByUrl
		Clarifai.getTagsByUrl(url, function (error, response) {
			if (error) {
				displayError(error)
			}
			else if (response) {
				try {
					// if we get the response display it
					displayTag(response)									
				}
				catch (error) {
					console.log(error)
				}
			}
		})

		/*
		 * TODO
		 * request colors for the same image from Clarifai
		 */

	})

	/*
	 * displayTag
	 * functionality to display the tag
	 */
	function displayTag (response) {
		console.log("Clarifai Response!")
		console.log(response)

		/* Getting the first image from the 
		 * we can give more than one image at the time of request
		 * but since we are requesting tags for only one image
		 * we only need to retrieve the first image*/
		var image 	= 	response.results[0]
		var tag 	= 	image.result.tag

		var conceptsLength = tag.classes.length

		// Looping through all the classes in the tag using map
		// to get the html for each concept
		var concepts = tag.classes.map(function (value, index, array) {
			var prob = Number.parseFloat(tag.probs[index]);
			var probPercentage = (prob * 100).toString() + "%";

			// Assigning color for the progressBar using `assignColor`
			var progressBarColor = assignColor(prob, index, conceptsLength)

			// Generating progress bar using values and a template
			var progressBar = `<div class="progress">
						<div class="progress-bar progress-bar-${progressBarColor}" role="progressbar" aria-valuenow="${prob}" aria-valuemin="0" aria-valuemax="100" style="width: ${probPercentage};">
							${prob}
						</div>
					</div>`

			// adding class and concept information
			return `<div class="row">
						<div class="col-sm-12">
							<h3>${value}</h3>
						</div>
					</div>`
		})

		// joining all the stuff generated and throwing the html into .tags
		tags.html(concepts.join(""))

		// displaying the hidden container
		tagsContainer.show()

	}


	/*
	 * assignColor
	 * returns the color for progress bar based on the probability
	 */
	function assignColor (prob, index, length) {
		if (prob > 0.9) {
			return "success"
		}
		else if (prob > 0.85 && prob <= 0.9) {
			return "info"
		}
		else {
			return "warning"
		}
	}

	// In case of error displays an error in the Clarifai 
	function displayError (error) {

		// Preparing the error message
		var errorMessage = "<p>" + error.status_msg + "</p>" 
		var errorHtml = "<div class='errorBox'><h1>Error ❌</h1>" + errorMessage + "</div>"

		// throwing the errorHtml in .tags
		tags.html(errorHtml)
		tagsContainer.show()
	}


	// function to initialize the keys
	function initialize () {
		var keys = getKeys() || {}

		var clientId = keys["CLARIFAI_CLIENT_ID"]
		var clientSecret = keys["CLARIFAI_CLIENT_SECRET"]

		if (!clientId || !clientSecret) {
			app.html("Enter your Clarifai's Client ID and Client Secret in order to successfully run this demo. Go to developer.clarifai.com, sign up and create your application if you haven't already. You'll have to edit keys.js file to enter your credentials")
			return;
		}

		Clarifai.initialize({
			clientId: clientId,
			clientSecret: clientSecret
		})
	}
}(jQuery, Clarifai));