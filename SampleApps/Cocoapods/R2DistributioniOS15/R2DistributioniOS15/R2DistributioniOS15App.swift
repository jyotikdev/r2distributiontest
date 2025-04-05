//
//  R2DistributioniOS15App.swift
//  R2DistributioniOS15
//
//  Created by Tamer Bader on 4/4/25.
//

import SwiftUI
import SquareMobilePaymentsSDK
import UIKit

class AppDelegate: NSObject, UIApplicationDelegate {

    var mobilePaymentSDK: SDKManager!

    func application(
        _ application: UIApplication,
        didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]? = nil
    ) -> Bool {
        print("Hey")
        MobilePaymentsSDK.initialize(
            applicationLaunchOptions: launchOptions,
            squareApplicationID: "sq0idp-2p247sPrashdnnY5BUu2AQ"
        )
        self.mobilePaymentSDK = MobilePaymentsSDK.shared
        return true
    }
}

@main
struct R2DistributioniOS15App: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}


enum Config {
    static let appID: String = ""
    static let locationId: String = ""
    static let accessToken: String = ""
}
