import XCTest

#if !os(macOS)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(elizabethsiegle_github_ioTests.allTests),
    ]
}
#endif