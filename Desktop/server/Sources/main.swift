import HeliumLogger //trivial logger
import Kitura //get whole kitura framework
import KituraStencil //enables Stencil for us
import LoggerAPI

let router = Router()
router.setDefault(templateEngine: StencilTemplateEngine()) //we can say render a page, use it for everything else after -> empty bc using new one
HeliumLogger.use() //.warning param don't show evrything below

let bios = [
    "Stephen" : "I like cats and halo top ice cream and steven universe and pubnub",
    "Kevin" : "lol AT&T",
    "Tomomi" : "I also like cats and gifs and nexmo and everything and everyone"
]

router.get("/") { //when this thing comes in, handle it here -> closure
    request, response, next in
    //pure HTML now in Swift
    defer { next() } //forget about it, will run byself, no need to remember later code
    try response.render("home", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up

} //get, post, maybe put

router.all("/static", middleware: StaticFileServer() ) //everything goes to static: doesn't care about what type of server. middleware = pipeline: sits in middle and jumps in when needed like authentication, encryption, compression: register ("I want to use something") then use autmatatically when needed

router.get("/staff") {
    request, response, next in
    defer { next() } //forget about it, will run byself, no need to remember later code
    
    try response.render("staff", context: [:])
}

router.get("/staff/:name") { //when this thing comes in, handle it here -> closure
    request, response, next in
    defer { next() } //forget about it, will run byself, no need to remember later code
    guard let name = request.parameters["name"] //optional, ensure valid name in URL
        else { return } //bail out, can't read in from req
    
    var context = [String: Any]() //can pass arrays, objects into Stencil will figure out can handle anything
    
    if let bio = bios[name] { //if in dictionary, pull out
        context["name"] = name //if works, put into context
        context["bio"] = bio
    }
    
    context["people"] = bios.keys.sorted() //string array returned
    //try response.render("lol", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up

    //response.send("meet our gr9 staff")
    try response.render("staff", context: context)
    
} //get, post, maybe put

router.get("/contact") { //all = broad, encompasses everything: get, post get: just get, no post
    request, response, next in
    response.send("get in touch with us")
    try response.render("lol", context: [:]) //whole closure automatically throws, no need for catch bc will bubble up
    next() //continue pipeline, onto next closure: comment out, avoid rest of stack
    
} //get, post, maybe put







Kitura.addHTTPServer(onPort:8030, with: router)
Kitura.run()
//code below won't run. ^never runs.
