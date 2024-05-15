Pod::Spec.new do |s|
  pod_name = 'R2Distribution'
  framework_filename = "#{pod_name}.xcframework"

  s.name = pod_name
  s.version = '2.0.0-beta1'
  s.license = 'Developer license'
  s.homepage = 'https://github.com/jyotikdev/r2distributiontest'
  s.authors = 'Square'

  s.summary = 'A testing framework'

  s.ios.deployment_target = '13.0'

  s.source = { :git => "https://github.com/jyotikdev/r2distributiontest.git", :tag => '2.0.0-beta1' }
 # s.vendored_frameworks = "XCFrameworks/R2Distribution"

end