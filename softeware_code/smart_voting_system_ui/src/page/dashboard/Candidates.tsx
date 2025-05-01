import { useContext } from "react";
import { AppContext } from "../../context/appContext";
import LoadingPhoto from "../../assets/loadingPhoto.svg";
export const Candidates = () => {
  const { candidates } = useContext(AppContext);

  if (!candidates)
    return (
      <div style={{ width: "100%", display: "grid", placeItems: "center" }}>
        <img src={LoadingPhoto} alt="loading..." />
      </div>
    );
  return (
    <div
      style={{
        width: "100%",
        maxWidth: 500,
        margin: "0 auto",
        padding: "0 10px",
      }}
    >
      <ul
        style={{
          listStyle: "none",
          display: "flex",
          flexDirection: "column",
          gap: 20,
        }}
      >
        {Object.values(candidates).map((candidate, i) => {
          return (
            <li
              key={i}
              style={{
                backgroundColor: "#141E46",
                display: "grid",
                placeItems: "center",
                borderRadius: "5px",
                color: "#fff",
                fontWeight: "bold",
                padding: 10,
              }}
            >
              {candidate as string}
            </li>
          );
        })}
      </ul>
    </div>
  );
};
