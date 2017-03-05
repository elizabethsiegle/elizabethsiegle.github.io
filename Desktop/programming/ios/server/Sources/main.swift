import HeliumLogger
import Kitura

HeliumLogger.use()
let router = Router()

Kitura.addHTTPServer(onPort: 8030, with: router)
Kitura.run()

//curl tells more info you cant see in web browser
//router.get("/hello") {
//    request, response, next in
//    defer { next() }
//    response.send("hello")
//}

//parse for us
//HANDLE FORMS
router.post("/employees/edit", middleware: BodyParser()) //post, handle to employees add path
router.post("/employees/add", middleware: BodyParser()) {
    //go through and add values: can not be before middleware -> let Kitura do tough work, 
    request, response, next in
    //no defer next bc force end connection immediately. not waiting for more instructions
    guard let values = request.body else { //attempt to parse values out
        //send back bad requests
        try response.status(.badRequest).end() //end = terminate connection immediately
        return
    }
    //if request
    //http land : paused body
    //attempt to convert values to .urlEncoded and if so, perform
    guard case .urlEncoded(let body) = values else { //slightly abusing swift in kitura to get this to work
        try response.status(.badRequest).end() //end = terminate connection immediately
        return //don't want wrong thing to get into server code
    }
    //at this point, we def have something from user. now trying to figure out what tis
    guard let name = body["name"] else { return }
    response.send("Adding new employee.. \(name)")
    next() //now safe. we trust this person.
    
}

router.post("/employees/edit") {
    request, response, next in
    guard let values = request.body else {
        try response.status(.badRequest).end()
        return
    }
    guard case .json(let body) = values else {
        try response.status(.badRequest).end()
        return
    }
    if let name = body["name"].string { //json picky, could be #: attempt to convert
        response.send("Edited employee \(name)") //kitura handles json very well for you though 
        //strinValue = full, string = optional
    }
    
}

router.get("/games/:name") { //form
    request, response, next in
    defer { next() }
    guard let name = request.parameters["name"] else { return }
    response.send("lets play the \(name) game")
}


router.get("/platforms") {
    request, response, next in
    
    guard let name = request.queryParameters["name"] else {
        try response.status(.badRequest).end()
        return
    }
    response.send("Loading the \(name) platform")
    next()
}

//router.get("/search([0-9+)/([A-Za-z\\+)" {
  //  /search/banana/2017
//}

//regex in asking Kitura to parse something for you = multiple ways to pass data
    //receive data in avariety of formats



router.get("/hello", handler: { //closure
    request, response, next in
    defer { next() }
    response.send("Hello")
    }, {
        request, response, next in
        defer { next() }
        response.send("World")
})
router.route("/test")
    .get() {
        request, response, next in
        defer { next() }
    }.post() {
        request, response, next in
        defer { next() }
        response.send("hello")
} //stack get, post in one routing call 

//import HeliumLogger //trivial logger
//import Kitura //get whole kitura framework
//import KituraStencil //enables Stencil for us
//import LoggerAPI

//let router = Router()
//router.setDefault(templateEngine: StencilTemplateEngine()) //we can say render a page, use it for everything else after -> empty bc using new one
//HeliumLogger.use() //.warning param don't show evrything below
//
//let bios = [
//    "Stephen" : "I like cats and halo top ice cream and steven universe and pubnub",
//    "Kevin" : "lol AT&T",
//    "Tomomi" : "I also like cats and gifs and nexmo and everything and everyone"
//]
//
//router.get("/") { //when this thing comes in, handle it here -> closure
//    request, response, next in
//    //pure HTML now in Swift
//    defer { next() } //forget about it, will run byself, no need to remember later code
//    try response.render("home", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up
//
//} //get, post, maybe put
//
//router.all("/static", middleware: StaticFileServer() ) //everything goes to static: doesn't care about what type of server. middleware = pipeline: sits in middle and jumps in when needed like authentication, encryption, compression: register ("I want to use something") then use autmatatically when needed
//
//router.get("/staff") {
//    request, response, next in
//    defer { next() } //forget about it, will run byself, no need to remember later code
//    
//    try response.render("staff", context: [:])
//}
//
//router.get("/staff/:name") { //when this thing comes in, handle it here -> closure
//    request, response, next in
//    defer { next() } //forget about it, will run byself, no need to remember later code
//    guard let name = request.parameters["name"] //optional, ensure valid name in URL
//        else { return } //bail out, can't read in from req
//    
//    var context = [String: Any]() //can pass arrays, objects into Stencil will figure out can handle anything
//    
//    if let bio = bios[name] { //if in dictionary, pull out
//        context["name"] = name //if works, put into context
//        context["bio"] = bio
//    }
//    
//    context["people"] = bios.keys.sorted() //string array returned
//    //try response.render("lol", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up
//
//    //response.send("meet our gr9 staff")
//    try response.render("staff", context: context)
//    
//} //get, post, maybe put
//
//router.get("/contact") { //all = broad, encompasses everything: get, post get: just get, no post
//    request, response, next in
//    response.send("get in touch with us")
//    try response.render("lol", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up
//    next() //continue pipeline, onto next closure: comment out, avoid rest of stack
//    
//} //get, post, maybe put
//
//
//
//
//
//
//
//Kitura.addHTTPServer(onPort:8030, with: router)
//Kitura.run()
////code below won't run. ^never runs.
