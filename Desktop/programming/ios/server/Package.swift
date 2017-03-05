import PackageDescription

let package = Package(
    name: "server",
    dependencies:  [
        //url to github repo usually + version #
        .Package(url: "https://github.com/ibm-swift/kitura.git", majorVersion: 1), //web stuff takes care of http, protocols, getting/receiving, sending, get/post web stuff we dont care about
        .Package(url: "https://github.com/ibm-swift/heliumlogger.git", majorVersion: 1),
        //simple almost trivial engine: tracks
        .Package(url: "https://github.com/ibm-swift/kitura-stenciltemplateengine.git", majorVersion: 1)
        //first of web techniques need to learn: let you create html freely in file nowhere near swift code= different zip codes, has some logic in there: loops, conditions
    ]
)
