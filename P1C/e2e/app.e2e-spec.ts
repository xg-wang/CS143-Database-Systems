import { P1CPage } from './app.po';

describe('p1-c App', function() {
  let page: P1CPage;

  beforeEach(() => {
    page = new P1CPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});
