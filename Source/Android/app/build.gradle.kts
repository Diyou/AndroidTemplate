plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
}

android {
    namespace = "com.diyou.dotcmake"
    compileSdk = 36
    ndkVersion = "29.0.13113456"

    defaultConfig {
        applicationId = "com.diyou.dotcmake"
        minSdk = 26
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        externalNativeBuild {
            cmake {
                arguments += "-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=${project.file("../../../.cmake/ToolChains/gradle/callback.cmake").absolutePath}"
            }
        }
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
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
    externalNativeBuild {
        cmake {
            path = file("../../../CMakeLists.txt")
            version = "4.0.2"
        }
    }
    buildFeatures {
        viewBinding = true
        prefab = true
    }
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)

    implementation(files("libs/SDL3/SDL3-3.2.14.aar"))
}