import { signInWithEmailAndPassword } from "firebase/auth";
import { ReactNode, createContext, useEffect, useState } from "react";
import { auth, db } from "../firebase/firebase-config";
import { onValue, ref } from "firebase/database";

interface AppContextType {
  candidates: any;
  voteList: any;
  voters: any;
  setAppData: (value: any) => void;
}

const defaultContext: AppContextType = {
  candidates: null,
  voteList: null,
  voters: null,
  setAppData: () => {},
};

export const AppContext = createContext<AppContextType>(defaultContext);

interface Props {
  children: ReactNode;
}

const AppProvider = ({ children }: Props) => {
  const [appData, setAppData] = useState(defaultContext);

  useEffect(() => {
    signInWithEmailAndPassword(auth, "votingsystem27@gmail.com", "123@voting")
      .then((userCredential) => {
        // Signed in
        const user = userCredential.user;

        const candidatesRef = ref(db, `/ElectionData/${user.uid}/candidates`);
        onValue(candidatesRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, candidates: data }));
        });

        const votersRef = ref(db, `/ElectionData/${user.uid}/voters`);
        onValue(votersRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, voters: data }));
        });

        const votesRef = ref(db, `/ElectionData/${user.uid}/voteList`);
        onValue(votesRef, (snapshot) => {
          const data = snapshot.val() || {};
          setAppData((prevState) => ({ ...prevState, voteList: data }));
        });
      })
      .catch((error) => {
        console.error("Error signing in: ", error.message);
      });
  }, []); // Dependencies array is empty to ensure this effect runs only once after the component mounts.

  return (
    <AppContext.Provider value={{ ...appData, setAppData }}>
      {children}
    </AppContext.Provider>
  );
};

export default AppProvider;
