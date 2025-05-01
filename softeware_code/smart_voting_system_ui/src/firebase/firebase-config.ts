// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";
import { getDatabase } from "firebase/database";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyB_GmRi5EKE3YCSDfAR8eywn8bwhGTsZYA",
  authDomain: "smart-voting-system-70e58.firebaseapp.com",
  databaseURL:
    "https://smart-voting-system-70e58-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "smart-voting-system-70e58",
  storageBucket: "smart-voting-system-70e58.appspot.com",
  messagingSenderId: "135233190708",
  appId: "1:135233190708:web:240bb396d70352ab280361",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
export const auth = getAuth(app);
export const db = getDatabase(app);
