// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "SquareMobilePaymentsSDK",
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
        .target(
            name: "MockReaderUI",
            dependencies: [
                .target(name: "MockReaderUIBinary", condition: .when(configuration: .debug))
            ]
        ),
        .binaryTarget(
            name: "MockReaderUIBinary",
            path: "XCFrameworks/MockReaderUI_2c76b3183c4f.zip"
        ),
    ]
)