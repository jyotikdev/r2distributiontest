// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "SquareMobilePaymentsSDK",
    platforms: [.iOS(.v15)],
    products: [
        .library(name: "SquareMobilePaymentsSDK", targets: ["SquareMobilePaymentsSDK"]),
        .library(name: "MockReaderUI", targets: ["MockReaderUI"]),
    ],
    dependencies: [],
    targets: [
        .binaryTarget(
            name: "SquareMobilePaymentsSDK",
            path: "XCFrameworks/SquareMobilePaymentsSDK_2c76b3183c4f.zip"
        ),
        .binaryTarget(
            name: "MockReaderUI",
            path: "XCFrameworks/MockReaderUI_2c76b3183c4f.zip"
        )
    ]
)