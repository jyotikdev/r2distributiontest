Pod::Spec.new do |s|
  pod_name = 'MobilePaymentsSDK'

  framework_filename = "#{pod_name}.xcframework"
  version_number = "v2.0.0-beta2"

  s.name = pod_name
  s.version = '2.0.0-beta2'
  s.license = 'Developer license'
  s.homepage = 'https://github.com/jyotikdev/r2distributiontest'
  s.authors = 'Jyoti & Tamer'

  s.summary = 'A testing framework'

  s.ios.deployment_target = '13.0'

  #s.source = { :http => "https://github.com/jyotikdev/r2distributiontest/releases/download/2.0.0-beta2/MobilePaymentsSDK.xcframework.zip"}

  s.source = { :git => "https://github.com/jyotikdev/r2distributiontest.git", :branch => 'jyoti/release' }
  s.vendored_frameworks = "XCFrameworks/#{framework_filename}"
  #s.vendored_frameworks = 'MobilePaymentsSDK.xcframework'
end