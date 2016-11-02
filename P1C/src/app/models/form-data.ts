export class FormData {
  entity: string;
  // people
  first: string;
  last: string;
  sex: string;
  dob: number;
  dod: number;
  // movie
  title: string;
  year: string;
  rating: string;
  company: string;
  genre: string[] = [];
  // review
  name: string;
  comment: string;
  // relation
  role: string;
}
