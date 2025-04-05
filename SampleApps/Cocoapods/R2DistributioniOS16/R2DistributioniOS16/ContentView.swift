//
//  ContentView.swift
//  R2DistributioniOS16
//
//  Created by Tamer Bader on 4/4/25.
//

import SwiftUI
import SquareMobilePaymentsSDK

struct ContentView: View {
    
    @State private var isAuthorized: Bool = false
    
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            if isAuthorized {
                Text("Mobile Payments SDK: Authorized")
            } else {
                Text("Mobile Payments SDK: Unauthorized")
            }
        }
        .padding()
        .onAppear {
            MobilePaymentsSDK.shared.authorizationManager.authorize(withAccessToken: Config.accessToken, locationID: Config.locationId) { error in
                isAuthorized = (MobilePaymentsSDK.shared.authorizationManager.state == .authorized)
            }
        }
    }
}

#Preview {
    ContentView()
}
