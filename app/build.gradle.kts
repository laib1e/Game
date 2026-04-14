plugins {
    alias(libs.plugins.android.application)
}

android {
    compileSdk = 36
    ndkVersion = "26.3.11579264"
    namespace = "com.example.game"

    defaultConfig {
        applicationId = "com.example.game"
        minSdk = 26
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"
        externalNativeBuild {
            cmake {
                abiFilters += "arm64-v8a"
                arguments += "-DANDROID_STL=c++_shared"
            }
        }
        buildFeatures.buildConfig = true;
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}